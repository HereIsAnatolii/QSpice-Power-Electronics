// Automatically generated C++ file on Sun Apr 28 11:19:36 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD f_point_x1.cpp kernel32.lib
#define WIDTH 32
#define FRAC 16
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


typedef struct
{
   unsigned int frac : 16; // number of fraction bits => 1 = 2^16
   int integ: 16;       // number of integral bits => 1 = 2^16
} F_PARTS;

typedef union
{
   long int all;           // the whole number
   F_PARTS part;        // integral/fractual parts
} F_POINT;

F_POINT in_var, ref_var, pwm_out;                     // inputs-outputs
F_POINT pi_out, P_term, P_gain, I_term, I_gain, err_var; // PI related
F_POINT temp0, temp1;                              // Temp variables

int scaler = 505; // based on the signal->PWM scaling factor

static double timestamp = 0.0;      // Interrupt related
double Tsamp = 1.0e-6;           // Interrupt sampling time

// int DllMain() must exist and return 1 for a process to load the .DLL
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

extern "C" __declspec(dllexport) void f_point_x1(void **opaque, double t, union uData *data)
{
   int  FBK  = data[0].i; // input Feedback
   int  REF  = data[1].i; // input Reference
   int &OUT  = data[2].i; // output

// Read the feedback value
   in_var.all = FBK;

// read the reference value as the integral part only
   ref_var.part.integ = REF;
   ref_var.part.frac = 0;

// Set the PI gains
   P_gain.all = 3277;   // 0.05 * 2^16 - need to be pre-calculated
   I_gain.all = 33;     // 500 * Ts * 2^16   - need to be pre-calculated

// PI controller sampled
   if((t-timestamp) >= Tsamp)
   {
      err_var.all = ref_var.all - in_var.all * scaler;      // Normalize the feedback value
      P_term.all = (err_var.part.integ*P_gain.all);         // Calculate the proportional part

      I_term.all += err_var.part.integ*I_gain.part.frac; // Calculate the integral part

      pi_out.all = P_term.all + I_term.all;
      pwm_out.all = pi_out.all * scaler;              // Scale-up the PI output for the PWM

      timestamp = t;
   }

   OUT = pwm_out.part.integ;
}
