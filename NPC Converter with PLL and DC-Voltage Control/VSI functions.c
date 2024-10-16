#define PI 3.14159
#define SQRT3 1.7320508075688772

#include <math.h>

int cnt_pll;
double angle, f_grid, delt, alpha, beta, a, b, c, da, db, dc;
double maxx, minn, offset;
double state_var[3] = {0.0,0.0,0.0};
double w;
typedef struct
{
	double in;
	double out;
} INOUT;

INOUT VA,VB,VC,A,B,C,d,q,Vd,Vq;

// CLARKE and INVERSE CLARKE transformations
void ABC2AB(double A, double B, double C, double *alpha, double *beta)
{
	*alpha = (2.0*A - B - C)/3.0;
	*beta = (B-C)/SQRT3;
}
void AB2ABC(double alpha, double beta, double *A, double *B, double *C)
{
	*A = alpha;
	*B = (SQRT3*beta - alpha)/2.0;
	*C =-(SQRT3*beta + alpha)/2.0;
}

// PARK and INVERSE PARK transformations
void AB2DQ(double alpha, double beta, double angle, double *d, double *q)
{
	*d = alpha*cos(angle) + beta*sin(angle);
	*q =-alpha*sin(angle) + beta*cos(angle);
}

void DQ2AB(double d, double q, double angle, double *alpha, double *beta)
{
	*alpha = d*cos(angle) - q*sin(angle);
	*beta  = d*sin(angle) + q*cos(angle);
}

void ABC2DQ(double A, double B, double C, double angle, double *d, double *q)
{
	double alpha, beta;
	
	ABC2AB(A,B,C,&alpha,&beta);
	AB2DQ(alpha,beta,angle,d,q);
}

void DQ2ABC(double d, double q, double angle, double *A, double *B, double *C)
{
	double alpha, beta;
	
	DQ2AB(d,q,angle,&alpha,&beta);
	AB2ABC(alpha,beta,A,B,C);
}

void maxmin(double A, double B, double C, double *maxx, double *minn)
{
	if(A>B&&A>C)	{*maxx = A;	} else
	if(B>A&&B>C)	{*maxx = B;	} else
						{*maxx = C;	}
						
	if(A<B&&A<C)	{*minn = A;	} else
	if(B<A&&B<C)	{*minn = B;	} else
						{*minn = C;	}
}
void DPWM60(double A, double B, double C, double *da, double *db, double *dc)
{
	double maxx,minn,offset;
	// find max and min
	maxmin(A,B,C,&maxx,&minn);
	if(maxx+minn > 0)	{		offset = 1 - maxx;	} else
							{		offset =-1 - minn;	}
	*da = A+offset;
	*db = B+offset;
	*dc = C+offset;
}

void PI_CONTROL(int EN, double Kp, double Ki, double Tsamp, double ref, double in, double div, double *Iterm, double *out)
{
	double error;
	
	if(EN == 1)
	{
		error = ref - in;
		*Iterm += error*Tsamp;
		*out = (Kp*error + Ki*(*Iterm))/div;
	}
	else
	{
		*Iterm = 0.0;
		*out = 0.0;
	}
}

void main()
{
/************* MEASUREMENTS *************/
/************* MEASUREMENTS *************/
/********** PLL/ANGLE CREATION **********/
	// PLL	
	// ABC-DQ Transformation
	ABC2DQ(VA.in,VB.in,VC.in,angle,&Vd.in,&Vq.in);
		// HPF VQ
	// PI CONTROL of PLL at Sampling 10xTsamp
	if(cnt_pll >= 9)
	{
		PI_CONTROL(1,0.17, 1, 10*TSAMP,  0.0, Vq.in, 1.0, &state_var[0], &Vq.out);
	} else {		cnt_pll++; Vq.out = 0.0;	}
	
	w = Vq.out + 2.0*PI*f_grid;
	// ANGLE CALCULATION
	if(angle >= 2*PI) {	angle = 0; }
	else {	angle += w * TSAMP; }
	
/********** PLL/ANGLE CREATION **********/
/****** TRANSFORMATION AND CONTROL ******/
	// ABC-DQ Transformation
	ABC2DQ(A.in,B.in,C.in,angle,&d.in,&q.in);
	
	// PI CONTROL
	PI_CONTROL(1, 3.33, 33333, TSAMP, 23.0, d.in, 400.0, &state_var[1], &d.out);
	PI_CONTROL(1, 3.33, 33333, TSAMP,  0.0, q.in, 400.0, &state_var[2], &q.out);

	// DQ-ABC Transformation
	DQ2ABC(d.out,q.out,angle,&A.out,&B.out,&C.out);
/****** TRANSFORMATION AND CONTROL ******/
/************** MODULATION **************/
	// DPWM
	DPWM60(A.out,B.out,C.out,&da,&db,&dc);
/************** MODULATION **************/
}

#include <ap_fixed.h>
#include <stdint.h>


typedef ap_fixed<64, 32, AP_TRN, AP_SAT> fpoint;
#define NULL  (fpoint)0.0
#define ONE   (fpoint)1.0
#define TWO   (fpoint)2.0
#define THREE (fpoint)3.0

#define SQRT3 (fpoint)1.7320508075688772

#define NULL (fpoint)0.0
#define PI (fpoint)3.14159
#define W (fpoint)2*PI*(fpoint)50

fpoint f_reset();
void dpwm_vsi(bool sync, bool en, fpoint Tsamp, fpoint *angle)
{
	/* sync - PWM sampling,
	 * en - enable the controller,
	 */
	#pragma HLS INTERFACE ap_ctrl_none port=return

	#pragma HLS INTERFACE ap_none port=sync
	#pragma HLS INTERFACE ap_none port=en
	#pragma HLS INTERFACE ap_none port=Tsamp
	#pragma HLS INTERFACE ap_none port=angle

	static bool prev_sync;
	fpoint angle_local;

	if (en == 0)
	{
		angle_local = f_reset();
	}

	if(sync == 1 && prev_sync == 0)
	{
		if(angle_local >= TWO*PI)	{	angle_local = NULL;	} else
						{	angle_local += W*Tsamp;		}
	}
	prev_sync = sync;
	*angle = PI;
}


fpoint f_reset()
{
	return NULL;
}
