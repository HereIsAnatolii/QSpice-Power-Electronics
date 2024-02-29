// Automatically generated C++ file on Fri Aug 25 15:49:07 2023
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD samp_test.cpp kernel32.lib
#include <math.h>
#define TSAMP 20.e-6
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

static double timecheck = 0.0;
static int sector;
static double max_p,max_n,max, min_p;

double findmax(double a, double b, double c);
double findmin(double a, double b, double c);
// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef VA
#undef VB
#undef VC
#undef SECTOR
#undef MAX
#undef ANGLE

extern "C" __declspec(dllexport) void matrix_control(void **opaque, double t, union uData *data)
{
   double an,bn,cn;
   double duty,angle,dutyA,dutyB,dutyC;

   double ap    = data[0].d;
   double bp    = data[1].d;
   double cp    = data[2].d;

/* Implement module evaluation code here: */

    if ( (t-timecheck) >= TSAMP)
    {
      timecheck = t;    // CHECK THE CURRENT TIME AND STORE IT
      an = -ap;   bn =-bp; cn =-cp;
      max_p = findmax(ap,bp,cp);
      max_n = findmax(an,bn,cn);

      max = findmax(0,max_p,max_n);

      if(max == ap)  {     duty = bn;        } else
      if(max == bp)  {     duty = cn;        } else
      if(max == cp)  {     duty = an;        } else

      if(max == an)  {     duty = -bp;       } else
      if(max == bn)  {     duty = -cp;       } else
                     {     duty = -ap;       }

      if(duty < 0)      angle = duty/max + 1;
      else             angle = duty/max;

      if(max == ap)  {     sector = 1;       } else
      if(max == bp)  {     sector = 3;       } else
      if(max == cp)  {     sector = 5;       } else

      if(max == an)  {     sector = 4;       } else
      if(max == bn)  {     sector = 6;       } else
                     {     sector = 2;       }

    data[3].d = sector;
    data[4].d = max;
    data[5].d = angle;
    }
}

/////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
void PI(int state, double ref_in, double sig_in, double Kp, double Ki, double Tsamp, double* sig_out)
{
   static double iterm;
   double err;

   // reset the integrator
   iterm = iterm*state;

   err = ref_in - sig_in;
   iterm += err*Tsamp;
   *sig_out = Kp*err + Ki*iterm;
}

void pwm(double sig, double car, double dt, int *H, int *L)
{
      if(sig>(car+dt)) { *H = 1; }
      else             { *H = 0; }

      if(sig<(car)) { *L = 1; }
      else             { *L = 0; }
}
void dq_abc(double d, double q, double theta, double * a, double * b, double * c)
{
   double pi = 3.14159;
   *a = cos(theta)*d - sin(theta)*q;
   *b = cos(theta-2.*pi/3.)*d - sin(theta-2.*pi/3.)*q;
   *c = cos(theta-4.*pi/3.)*d - sin(theta-4.*pi/3.)*q;
}
void abc_dq(double a, double b, double c, double theta, double * d, double * q)
{
   double pi = 3.14159;
   double k = 2.0/3.0;
   *d = k*( cos(theta)*a + cos(theta - 2.*pi/3.)*b + cos(theta - 4.*pi/3.)*c);
   *q = k*(-sin(theta)*a - sin(theta - 2.*pi/3.)*b - sin(theta - 4.*pi/3.)*c);
}

double findmax(double a, double b, double c)
{
   double max;

   if(a > b && a > c)
      max = a;
   else if(b > a && b > c)
      max = b;
   else
      max = c;

   return max;
}

double findmin(double a, double b, double c)
{
   double min;

   if(a < b && a < c)
      min = a;
   else if(b < a && b < c)
      min = b;
   else
      min = c;

   return min;
}
