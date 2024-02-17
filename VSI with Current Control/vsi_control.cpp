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


static double Dref = 0.0;
static double timecheck = 0.0;
static double angle = 0.0;
static double mA,mB,mC;
static double a,b,c;


void pwm(double sig, double car, double dt, int *H, int *L);
void dq_abc(double d, double q, double theta, double * a, double * b, double * c);
void abc_dq(double a, double b, double c, double theta, double * d, double * q);
void PI(int state, double ref_in, double sig_in, double Kp, double Ki, double Tsamp, double* sig_out);
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

extern "C" __declspec(dllexport) void vsi_control(void **opaque, double t, union uData *data)
{
   double Id, Iq,Vd,Vq;
   double d,q;
   double pll;

   double KP = 16.66;
   double KI = 8.33;
   int pAH, pBH, pCH;
   int pAL, pBL, pCL;

   double Ia    = data[0].d;
   double Ib    = data[1].d;

   double Va    = data[2].d;
   double Vb    = data[3].d;

   double Ic = -(Ia+Ib);
   double Vc = -(Va+Vb);
/* Implement module evaluation code here: */

    if ( (t-timecheck) >= 20.e-6)
    {
      timecheck = t;    // CHECK THE CURRENT TIME AND STORE IT

      // Convert input voltages to dq frame
      abc_dq(Va, Vb, Vc, angle, &Vd, &Vq);

      // PI controller to lock the loop
      PI(1,  0.0, Vq, 1, 1, 20.e-6, &pll);

      pll += 314.0;
      angle += pll * 20.e-6;

      if(angle > 3.14)
         angle = -3.14;

      // Convert input currents to dq frame
      abc_dq(Ia, Ib, Ic, angle, &Id, &Iq);

      if(t < 0.06)
         Dref = 100.0;
      else
         Dref = 300.0;

      PI(1, Dref, Id, KP, KI, 20.e-6, &d);
      PI(1,  0.0, Iq, KP, KI, 20.e-6, &q);

      dq_abc(d, q, angle, &a, &b, &c);

      mA = a/400.;
      mB = b/400.;
      mC = c/400.;

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

    data[4].d = mA;
    data[5].d = mB;
    data[6].d = mC;
    data[7].d = angle;
    data[8].d = Dref;
    data[9].d = 0.0;
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
