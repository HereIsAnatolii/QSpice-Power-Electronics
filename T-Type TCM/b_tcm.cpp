// Automatically generated C++ file on Thu Mar  7 13:21:21 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD b_tcm.cpp kernel32.lib
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

enum TCM_TYPE {
   TCM, B_TCM, S_TCM
};

double calc_freq(enum TCM_TYPE type, double M, double Vdc, double L, double t, double Imax, double fmax, double phase);
// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef fsw_a
#undef fsw_b
#undef fsw_c

extern "C" __declspec(dllexport) void b_tcm(void **opaque, double t, union uData *data)
{
   double  VdcParam = data[0].d; // input parameter
   double  Rparam   = data[1].d; // input parameter
   double  Lparam   = data[2].d; // input parameter
   double  Mparam   = data[3].d; // input parameter
   double  fparam   = data[4].d; // input parameter

   double &fsw_a    = data[5].d; // output
   double &fsw_b    = data[6].d; // output
   double &fsw_c    = data[7].d; // output

// Implement module evaluation code here:
   double M = Mparam;
   double Vdc = VdcParam;
   double L = Lparam;
   double R = Rparam;
   double Imax = 0.5*M*Vdc/R;
   double fmax = fparam;

   fsw_a = fmax*calc_freq(TCM,M, Vdc, L, t, Imax, fmax, 0.0);
   fsw_b = fmax*calc_freq(TCM,M, Vdc, L, t, Imax, fmax, -6.28/3.0);
   fsw_c = fmax*calc_freq(TCM,M, Vdc, L, t, Imax, fmax,  6.28/3.0);

}

double calc_freq(enum TCM_TYPE type, double M, double Vdc, double L, double t, double Imax, double fmax, double phase)
{
   double MM, div, r;
   double w = 314.159;

   MM = M*sin(w*t+phase);
   if(type == S_TCM)
   {
      div = 0.8*fabs(sin(w*t+phase))+0.2;

      r = Vdc/(8.0*L*Imax)*(1.0-MM*MM);
      if(r>=1)
         return (1.0-MM*MM);
      else
         return r;
   } else
   if(type == B_TCM)
   {
      div = 0.8*fabs(sin(w*t+phase))+0.2;

      r = Vdc/(8.0*L*fmax*Imax)*(1-MM*MM)/div;
      if(r>=1)
         return 1.0;
      else
         return r;
   } else
   {
      div = 0.8*fabs(sin(w*t+phase))+0.2;
      r = Vdc/(8.0*L*fmax*Imax)*(1-MM*MM)/div;
      return r;
   }
}
