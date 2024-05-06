// Automatically generated C++ file on Tue Mar  5 16:39:37 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD f_variable.cpp kernel32.lib

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

struct var_pwm
{
   double carrier;
   double timestep;
   double Ts;
   bool dir;
};


struct var_pwm pwm_a, pwm_b, pwm_c;
// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef fsw_a
#undef fsw_b
#undef fsw_c
#undef tri_a
#undef tri_b
#undef tri_c

double make_tri(int start,double fsw_ref, double f_max, double t, struct var_pwm *pwm);
extern "C" __declspec(dllexport) void variable_freq(void **opaque, double t, union uData *data)
{
   double  fsw_a  = data[0].d; // input
   double  fsw_b  = data[1].d; // input
   double  fsw_c  = data[2].d; // input
   double  fparam = data[3].d; // input parameter
   double &tri_a  = data[4].d; // output
   double &tri_b  = data[5].d; // output
   double &tri_c  = data[6].d; // output

// Implement module evaluation code here:
   double f_max = fparam;

   tri_a = make_tri(1, fsw_a, f_max, t, &pwm_a);
   tri_b = make_tri(1, fsw_b, f_max, t, &pwm_b);
   tri_c = make_tri(1, fsw_c, f_max, t, &pwm_c);
}

double make_tri(int start,double fsw_ref, double f_max, double t, struct var_pwm *pwm)
{
   double car_temp;
   if(start == 0)
   {
      pwm->carrier = 0.0;
      pwm->timestep = 0.0;
      pwm->dir = 0;
   }
   pwm->Ts = 1.0/fsw_ref;
   if(pwm->carrier >= pwm->Ts/1.0)
   {
      pwm->dir = !pwm->dir;
      pwm->timestep = t;
   }

   pwm->carrier = (t-pwm->timestep);
   if(pwm->dir == 0)
   {
      car_temp = (pwm->carrier*fsw_ref);
   } else {
      car_temp = (1-pwm->carrier*fsw_ref);
   }

   return 2*car_temp-1;
}
