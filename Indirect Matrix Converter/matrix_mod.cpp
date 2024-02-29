// Automatically generated C++ file on Mon Feb 12 23:14:26 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD matrix_mod.cpp kernel32.lib
#include <math.h>

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

static double theta = 0.0;
static double timecheck = 0.0;
void dq_abc(double d, double q, double theta, double * a, double * b, double * c);
// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef SECTOR
#undef ANGLE
#undef MAX
#undef A
#undef B
#undef C
#undef mA
#undef mB
#undef mC
#undef TRI

double wt = 200.0;

extern "C" __declspec(dllexport) void matrix_mod(void **opaque, double t, union uData *data)
{
   double SECTOR  = data[0].d; // input
   double ANGLE   = data[1].d; // input
   double MAX     = data[2].d;
   double TRI     = data[3].d;

   // Have to implement individual vectors to set the overlapping
   int sector, vector_1, vector_2, vector_3, vector_a,vector_b,vector_c;

   double phase_a,phase_b,phase_c;

   // Implement module evaluation code here:
   if(ANGLE-0.005>TRI)  vector_1 = 1;
   else vector_1 = 0;

   if(ANGLE>TRI)  vector_2 = 1;
   else           vector_2 = 0;

   if ((t - timecheck) >= 10.0e-6)
   {
      timecheck = t;
      if(theta>=6.28)
      {
         theta -= 6.28;
      } else
      {
         theta += wt*10.0e-6;
      }

   }


   int vector_A[6][2] = {  { 1, 1},{ 1, 0},{-1, 0},{-1,-1},{ 0,-1},{ 0, 1} };
   int vector_B[6][2] = {  { 0,-1},{ 0, 1},{ 1, 1},{ 1, 0},{-1, 0},{-1,-1} };
   int vector_C[6][2] = {  {-1, 0},{-1,-1},{ 0,-1},{ 0, 1},{ 1, 1},{1,0}};

   sector = (int)SECTOR-1;


   if(sector == 1)   {  vector_a = vector_1; vector_b = vector_2; vector_c = vector_2;   } else
   if(sector == 2)   {  vector_a = vector_1; vector_b = vector_2; vector_c = vector_2;   } else
   if(sector == 3)   {  vector_a = vector_2; vector_b = vector_1; vector_c = vector_2;   } else
   if(sector == 4)   {  vector_a = vector_2; vector_b = vector_1; vector_c = vector_2;   } else
   if(sector == 5)   {  vector_a = vector_2; vector_b = vector_2; vector_c = vector_1;   } else
                     {  vector_a = vector_2; vector_b = vector_2; vector_c = vector_1;   }

   data[4].d = vector_A[sector][vector_a];
   data[5].d = vector_B[sector][vector_b];
   data[6].d = vector_C[sector][vector_c];


   if(t > 70e-3)
   {
      wt = 1000.0;
      dq_abc(0.9, 0, theta, &phase_a, &phase_b, &phase_c);
   }
   else
   {
      wt = 120.0;
      dq_abc(0.25, 0, theta, &phase_a, &phase_b, &phase_c);
   }

      data[7].d = phase_a;//*MAX/325;
      data[8].d = phase_b;//*MAX/325;
      data[9].d = phase_c;//*MAX/325;

}

void dq_abc(double d, double q, double theta, double * a, double * b, double * c)
{
   double pi = 3.14159;
   *a = cos(theta)*d - sin(theta)*q;
   *b = cos(theta-2.*pi/3.)*d - sin(theta-2.*pi/3.)*q;
   *c = cos(theta-4.*pi/3.)*d - sin(theta-4.*pi/3.)*q;
}
