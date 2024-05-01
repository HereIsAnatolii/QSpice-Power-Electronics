// Automatically generated C++ file on Wed May  1 15:10:57 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD cordic.cpp kernel32.lib

union uData
{
   bool b;
   char c;
   unsigned char uc;
   short s;
   unsigned short us;
   int i;
   unsigned int ui;
   float f;
   double d;
   long long int i64;
   unsigned long long int ui64;
   char *str;
   unsigned char *bytes;
};

int angle = 0, sin_sig = 0, cos_sig = 0;
void CORDIC(int mag, int angle, int *sin, int *cos);
// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef SIN
#undef COS
#undef ANGLE

extern "C" __declspec(dllexport) void cordic(void **opaque, double t, union uData *data)
{
   int &SIN  = data[0].i; // output
   int &COS = data[1].i; // output
   int &ANGLE = data[2].i; // output

// Implement module evaluation code here:
   if(angle >= 3217<<2) // 2*pi Q11
   {
      angle = 0;
   }
   else
   {
      angle += 12;
   }

   CORDIC(0x7ff, angle, &sin_sig, &cos_sig);
   SIN = sin_sig;
   COS = cos_sig;
   ANGLE = angle;
}
void CORDIC(int mag, int angle, int *sin, int *cos)
{
   int k,i;
   int lut[12] = {1608,950, 501, 255, 128, 64, 32, 16, 8, 4, 2, 1};

   static int x[12],y[12],z[12];
   int pi_half = 3217;

   if(angle < -pi_half)
   {
      k = 0;
      z[0] = angle + (pi_half<<1);
   } else
   if(angle < pi_half)
   {
      k = 1;
      z[0] = angle;
   } else
   if(angle < (pi_half<<1))
   {
      k = 2;
      z[0] = angle-pi_half;
   } else
   if(angle < pi_half+(pi_half<<1))
   {
      k = 3;
      z[0] = angle-(pi_half<<1);
   } else {
      k = 4;
      z[0] = angle-(pi_half<<2);
   }

   x[0] = mag;
   y[0] = 0;

   for(i=0; i<12; i++)
   {
      if(z[i]<0)
      {
         x[i+1] = x[i] + (y[i]>>i);
         y[i+1] = y[i] - (x[i]>>i);
         z[i+1] = z[i] + lut[i];
      } else {
         x[i+1] = x[i] - (y[i]>>i);
         y[i+1] = y[i] + (x[i]>>i);
         z[i+1] = z[i] - lut[i];
      }
   }
   if(k == 0)
   {
      *sin = -y[12];
      *cos = -x[12];
   } else
   if(k == 1)
   {
      *sin = y[12];
      *cos = x[12];
   } else
   if(k == 2)
   {
      *sin  = x[12];
      *cos = -y[12];
   } else
   if(k == 3)
   {
      *sin  = -y[12];
      *cos = -x[12];
   } else {
      *sin = y[12];
      *cos =x[12];
   }
}
