// Automatically generated C++ file on Fri Aug 25 15:49:07 2023
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD samp_test.cpp kernel32.lib
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


static int dc_loop = 5;
static double Dref = 0.0;
static double timecheck = 0.0;
static double angle = 0.0;
static double mA,mB,mC,MAX,MIN,SV;
static double a,b,c;


void pwm(double sig, double car, double dt, int *H, int *L);
void dq_abc(double d, double q, double theta, double * a, double * b, double * c);
void abc_dq(double a, double b, double c, double theta, double * d, double * q);
void PI(int state, double ref_in, double sig_in, double Kp, double Ki, double Tsamp, double* sig_out);
double max(double A, double B, double C);
double min(double A, double B, double C);
// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef I_A
#undef I_B
#undef V_A
#undef V_B
#undef OUT_A
#undef OUT_B
#undef OUT_C
#undef PLL
#undef DREF
#undef QREF
#undef V_DC

extern "C" __declspec(dllexport) void npc_control(void **opaque, double t, union uData *data)
{
   double Vdc_ref;
   double Id, Iq,Vd,Vq;
   double d,q;
   double pll;
   double div;
   double KP = 16.66;
   double KI = 8.33;
   int pAH, pBH, pCH;
   int pAL, pBL, pCL;

   double Ia    = data[0].d;
   double Ib    = data[1].d;

   double Va    = data[2].d;
   double Vb    = data[3].d;
   double Vdc   = data[4].d;

   double Ic = -(Ia+Ib);
   double Vc = -(Va+Vb);
/* Implement module evaluation code here: */

    if(t < 25e-3) {  Vdc_ref = 750; }
    else          {  Vdc_ref = 800; }
    if ( (t-timecheck) >= 20.e-6)
    {
      timecheck = t;    // CHECK THE CURRENT TIME AND STORE IT
      if(dc_loop > 4)
      {
         dc_loop = 0;
         PI(1, Vdc_ref, Vdc, 9.52, 25, 100.e-6, &Dref);

      }
      else
      {
         dc_loop++;
      }

         // Convert input voltages to dq frame
         abc_dq(Va, Vb, Vc, angle, &Vd, &Vq);

         // PI controller to lock the loop
         PI(1,  0.0, Vq, 10, 10, 20.e-6, &pll);

         pll += 314.0;

         if(angle > 3.14)
         {
            angle = -3.14;
         }
         else if (angle <-3.14)
         {
            angle = -3.14;
         }
         else
         {
            angle += pll * 20.e-6;
         }
      // Convert input currents to dq frame
      abc_dq(Ia, Ib, Ic, angle, &Id, &Iq);

//      if(Dref>50) Dref=50;
      PI(1, Dref, Id, KP, KI, 20.e-6, &d);
      PI(1,  0.0, Iq, KP, KI, 20.e-6, &q);

      dq_abc(d, q, angle, &a, &b, &c);

      if(Vdc>1)      {         div = 0.5*Vdc;      }
      else           {         div = 0.5;          }
      mA = a/div;
      mB = b/div;
      mC = c/div;

      MAX = max(mA,mB,mC);
      MIN = min(mA,mB,mC);

      mA -= (MAX+MIN)/2.0;
      mB -= (MAX+MIN)/2.0;
      mC -= (MAX+MIN)/2.0;

    }
    if(mA > 0.95)
       mA = 0.95;
    else if(mA <-0.95)
       mA = -0.95;
    if(mB > 0.95)
       mB = 0.95;
    else if(mB <-0.95)
       mB = -0.95;
    if(mC > 0.95)
       mC = 0.95;
    else if(mC <-0.95)
       mC = -0.95;

    data[5].d = mA;
    data[6].d = mB;
    data[7].d = mC;
    data[8].d = angle;
    data[9].d = Dref;
    data[10].d = 0.0;
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

double max(double A, double B, double C)
{
   double MAX;
   if(A>B && A>C)   { MAX = A;   } else
   if(B>C && B>A)   { MAX = B;   } else
                    { MAX = C;   }
   return MAX;
}



double min(double A, double B, double C)
{
   double MIN;
   if(A<B && A<C)   { MIN = A;   } else
   if(B<C && B<A)   { MIN = B;   } else
                    { MIN = C;   }
   return MIN;
}
