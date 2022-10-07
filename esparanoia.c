/*----------------------------------------------------------------
 *	TAB STOPS SET TO 5.
 *
 *	Re-written version of paranoia with interactivity removed to be
 *	suitable for running on any system compliant with ISO C90, C99
 *	and C++99, and in particular for embedded control systems which
 *	only implement the minimum requirement.
 *
 *	If you're anxious to get stuck in immediately, just jump to
 *	the string TODO below.
 *	
 *	Cleaned up, converted to non-interactivity and extended somewhat
 *	by:-
 *	
 *		Professor Les Hatton
 *		Computing, Information Systems and Mathematics
 *		University of Kingston
 *		Kingston on Thames, KT1 2EE
 *		UK
 *
 *		L.Hatton@kingston.ac.uk, lesh@oakcomp.co.uk
 *
 *		Please send any results you get to me at the above address.
 *		===========================================================
 *
 *	I'm adding some of my own favourite gotchas over the years but
 *	the real work was done by the following original authors:-
 *
 *	(C) Apr 19 1983 in BASIC version by:
 *		Professor W. M. Kahan,
 *		567 Evans Hall
 *		Electrical Engineering & Computer Science Dept.
 *		University of California
 *		Berkeley, California 94720
 *		USA
 *	
 *	converted to Pascal by:
 *		B. A. Wichmann
 *		National Physical Laboratory
 *		Teddington Middx
 *		TW11 OLW
 *		UK
 *	
 *	converted to C by:
 *		David M. Gay		and		Thos Sumner
 *		AT&T Bell Labs				Computer Center, Rm. U-76
 *		600 Mountain Avenue			University of California
 *		Murray Hill, NJ 07974		San Francisco, CA 94143
 *		USA						USA
 *
 *	Various assumptions have been made about the maturity of C
 *	compiling now which I hope are true, for example, that function
 *	prototypes are now universal.  The code is compliant with
 *	ISO C90, C99 and C++99 and survived Safer C.  It is also silent
 *	under the -Wall option of gcc.
 *
 *	Revision:	$Revision: 1.1 $
 *	Date:	$Date: 2019/12/12 14:19:23 $
 *
 *	Style 	exdented braces + full function prototypes.
 *			Tab set by default to 5.
 *
 *	23/09/05	LH	Typographical error corrected.
 *	24/03/04	SD	LONG_DOUBLE test option added.
 *				Sanity check for calculated T1 == EPSILON added.
 *				Simon Dalley and Adrian Capel, UKAEA Culham
 *	19/09/03	LH	Initial release.
 *
 ----------------------------------------------------------------*/
#include <stdlib.h>
#include <float.h>

#ifndef	NOSTDIO
#include <stdio.h>
#endif

#ifndef 	NOSIGNAL
#include <signal.h>
#endif

#ifndef 	NOSETJMP
#	include <setjmp.h>
	jmp_buf ovfl_buf;
#else
/*
 *	This supplied setjmp never jumps.
 */
	char		a;				/*	Dummy	*/
	char * 	ovfl_buf = &a;		/*	Dummy	*/

	int
	setjump( void * buf )
	{
		return 0;
	}
#endif

typedef void (*Sig_type)(int);
Sig_type sigsave;

#ifdef SINGLE
extern double fabs(double), floor(double), log(double);
extern double pow(double, double), sqrt(double), exp(double);
#	define FLOAT float
#	define EPSILON (FLT_EPSILON)
#	define FABS(x) (float)fabs((double)(x))
#	define FLOOR(x) (float)floor((double)(x))
#	define LOG(x) (float)log((double)(x))
#	define POW(x,y) (float)pow((double)(x),(double)(y))
#	define SQRT(x) (float)sqrt((double)(x))
#	define EXP(x) (float)exp((double)(x))
#ifndef 	NOTRANSCENDENTAL
extern double sin(double), cos(double), tan(double);
extern double asin(double), acos(double), atan(double);
#	define SIN(x) (float)sin((double)(x))
#	define COS(x) (float)cos((double)(x))
#	define TAN(x) (float)tan((double)(x))
#	define ASIN(x) (float)asin((double)(x))
#	define ACOS(x) (float)acos((double)(x))
#	define ATAN(x) (float)atan((double)(x))
#endif
#ifndef 	NOHYPERBOLIC
extern double sinh(double), cosh(double), tanh(double);
#	define SINH(x) (float)sinh((double)(x))
#	define COSH(x) (float)cosh((double)(x))
#	define TANH(x) (float)tanh((double)(x))
#endif
#else
#ifdef LONG_DOUBLE
extern long double fabsl(long double), floorl(long double);
extern long double logl(long double), powl(long double, long double);
extern long double sqrtl(long double), expl(long double);
#	define FLOAT long double
#	define EPSILON (LDBL_EPSILON)
#	define FABS(x) fabsl(x)
#	define FLOOR(x) floorl(x)
#	define LOG(x) logl(x)
#	define POW(x,y) powl(x,y)
#	define SQRT(x) sqrtl(x)
#	define EXP(x) expl(x)
#ifndef 	NOTRANSCENDENTAL
extern long double sinl(long double), cosl(long double), tanl(long double);
extern long double asinl(long double), acosl(long double), atanl(long double);
#	define SIN(x) sinl(x)
#	define COS(x) cosl(x)
#	define TAN(x) tanl(x)
#	define ASIN(x) asinl(x)
#	define ACOS(x) acosl(x)
#	define ATAN(x) atanl(x)
#endif
#ifndef 	NOHYPERBOLIC
extern long double sinhl(long double), coshl(long double), tanhl(long double);
#	define SINH(x) sinhl(x)
#	define COSH(x) coshl(x)
#	define TANH(x) tanhl(x)
#endif
#else
extern double fabs(double), floor(double), log(double);
extern double pow(double, double), sqrt(double), exp(double);
#	define FLOAT double
#	define EPSILON (DBL_EPSILON)
#	define FABS(x) fabs(x)
#	define FLOOR(x) floor(x)
#	define LOG(x) log(x)
#	define POW(x,y) pow(x,y)
#	define SQRT(x) sqrt(x)
#	define EXP(x) exp(x)
#ifndef 	NOTRANSCENDENTAL
extern double sin(double), cos(double), tan(double);
extern double asin(double), acos(double), atan(double);
#	define SIN(x) sin(x)
#	define COS(x) cos(x)
#	define TAN(x) tan(x)
#	define ASIN(x) asin(x)
#	define ACOS(x) acos(x)
#	define ATAN(x) atan(x)
#endif
#ifndef 	NOHYPERBOLIC
extern double sinh(double), cosh(double), tanh(double);
#	define SINH(x) sinh(x)
#	define COSH(x) cosh(x)
#	define TANH(x) tanh(x)
#endif
#endif
#endif

static	FLOAT Radix, BInvrse, RadixD2, BMinusU2;
/*
 *	Small floating point constants.
 */
static	FLOAT Zero 		= 0.0;
static	FLOAT Half 		= 0.5;
static	FLOAT One 		= 1.0;
static	FLOAT Two 		= 2.0;
static	FLOAT Three 		= 3.0;
static	FLOAT Four 		= 4.0;
static	FLOAT Five 		= 5.0;
static	FLOAT Eight 		= 8.0;
static	FLOAT Nine 		= 9.0;
static	FLOAT TwentySeven 	= 27.0;
static	FLOAT ThirtyTwo 	= 32.0;
static	FLOAT TwoForty 	= 240.0;
static	FLOAT MinusOne 	= -1.0;
static	FLOAT OneAndHalf 	= 1.5;
/*
 *	From Abramowitz and Stegun.
 */
static	FLOAT SIN_ONE 		= 0.84147098480789650665250;
static	FLOAT COS_ONE 		= 0.54030230586813971740094;
static	FLOAT MATH_PI 		= 3.141592653589793238462643;
static	FLOAT MATH_E 		= 2.718281828459045235360287;

static	FLOAT min_val;
/*
 *	Integer constants.
 */
static	int 	NoTrials 		= 20; /*	Number of tests for commutativity.	*/
/*
 *	Useful Boolean things.
 */
#define FALSE 		0
#define TRUE 		1

/*
 *	Definitions for declared types 
 *	Guard 	== (YES, NO);
 *	Rounding 	== (CHOPPED, ROUNDED, OTHER);
 *	Message 	== packed array [1..40] of char;
 *	Class 	== (FLAW, DEFECT, SERIOUS, FAILURE);
 *				(Increasing order of severity).
 */
#define YES 		1
#define NO  		0

#define CHOPPED 	2
#define ROUNDED 	1
#define OTHER   	0
/*
 *	This is the maximum size of the character buffer used for
 *	integer and floating conversion in the message() function.
 */
#define MAXCHAR	50
/*
 *	paranoia failure modes in decreasing order of severity.  FAILURE is
 *	the most serious.
 */
typedef	enum	{
	FAILURE,
	SERIOUS,
	DEFECT,
	FLAW
}	Class;

typedef 	int 		Guard, Rounding;
typedef 	char 	Message;
/*
 *	Storage variables for max and min errors found in additional numerical
 *	calculations.
 */
static	FLOAT	MINERR_DiffTran1;	/*	sin**2 + cos**2 = 1.0		*/
static	FLOAT	MAXERR_DiffTran1;

static	FLOAT	MINERR_DiffTran2;	/*	Abramowitz & Stegun 4.3.29	*/
static	FLOAT	MAXERR_DiffTran2;

static	FLOAT	MINERR_DiffExpLog;	/*	exp(log(X)) - X			*/
static	FLOAT	MAXERR_DiffExpLog;

static	FLOAT	MINERR_DiffHyper1;	/*	cosh**2(X) - sinh**2(X) = 1.0	*/
static	FLOAT	MAXERR_DiffHyper1;
/*
 *	General working variables.
 */
static	FLOAT 	AInvrse, A1;
static	FLOAT 	C, CInvrse;
static	FLOAT 	D, FourD;
static	FLOAT 	E0, E1, Exp2, E3, MinSqEr;
static	FLOAT 	SqEr, MaxSqEr, E9;
static	FLOAT 	Third;
static	FLOAT 	F6, F9;
static	FLOAT 	H, HInvrse;
static	FLOAT 	StickyBit, J;
static	FLOAT 	MyZero;
static	FLOAT 	Precision;
static	FLOAT 	Q, Q9;
static	FLOAT 	R, Random9;
static	FLOAT 	T, Underflow, S;
static	FLOAT 	OneUlp, UfThold, U1, U2;
static	FLOAT	EpsilonOverU1;
static	FLOAT 	V, V0, V9;
static	FLOAT 	W;
static	FLOAT 	X, X1, X2, X8, Random1;
static	FLOAT 	Y, Y1, Y2, Random2;
static	FLOAT 	Z, PseudoZero, Z1, Z2, Z9;

static	int 		plevel;
static	int 		Indx;
static	int 		I;
static	int 		II;
static	int 		ErrCnt[4];
static	int 		fpecount;
static	int 		Milestone;
static	int 		PageNo;
static	int 		M, N, N1;
static	int 		Break, Done, NotMonot, Monot, Anomaly, IEEE,
				SqRWrng, UfNGrad;

static	Guard 	GMult, GDiv, GAddSub;
static	Rounding 	RMult, RDiv, RAddSub, RSqrt;

/*****************************************************************
 *	This function is the only one which should need supplying and then
 *	only if <stdio.h> is not available in which case the flag
 *	-D NOSTDIO should be used in the compilation and some way
 *	of getting string back to the target needs to be supplied
 *	at the points marked TODO.
 *****************************************************************/
/*
 *	Get a string to the outside world somehow.
 */
void
message( int append_nl, char * string )
{
/*
 *	Guard - passing NULL to the C run-time library can seriously
 *	damage your health.
 */
	if ( string == NULL )	return;
/*						=======			*/
/*
 *	If stdio is present, use it gratefully.
 */
#ifndef	NOSTDIO
	printf("%s", string);
#else
/*
 *	TODO
 *	Do what you can with local facilities for sending a stream
 *	of characters in the character variable 'string' to the outside world.
 */
#endif
	if ( append_nl )
	{
#	ifndef	NOSTDIO
		printf("\n");
#	else
/*
 *	TODO
 *	Do what you can with local facilities for sending a newline
 *	to the outside world.
 */
#	endif
	}
}
/*****************************************************************
 *	You should not need to adjust anything below here.
 *****************************************************************/
/*
 *	Very cheap and nasty FLOAT to string conversion.
 *	It finds the exponent the hard way and then prints out
 *	the format as +/- .XXXXXXXXE[+/-]YYY
 *	where there are m decimal places and an exponent YYY.
 *
 *	Note that this implementation cannot distinguish between
 *	an Inf (1/0) and a NaN (0/0).
 *
 *	dd			INPUT: double to convert
 *	m			INPUT: Number of decimal places
 *	min_10_exp	INPUT: smallest allowable power of 10.
 *	max_10_exp	INPUT: largest  allowable power of 10.
 *	smallest		INPUT: smallest allowable value which is power of 10.
 *	st			OUTPUT: Output string.
 */
char *
local_float2string(	FLOAT dd, int m, int min_10_exp, int max_10_exp,
				FLOAT smallest, char * st )
{
	int		is		= 0;
	int		i;
	int		x		= 0;
	int		exponent	= 0;
	int		absexp	= 0;

	FLOAT	d 		= smallest;
	FLOAT	absdd;
/*
 *	Step 1.  Walk up from the tiniest number to find the first exponent
 *	which will give a value of the mantissa between 0 and 1.
 */
	absdd	= (( dd < 0.0) ? (-dd) : (dd));

	for( i = min_10_exp; i < max_10_exp; ++i )
	{
		d *= 10.0;
		++x;

		if ( d > absdd )
		{
			break;
/*			======				*/
		}
	}

	if ( i < max_10_exp )
	{
/*
 *		The break in the above loop was taken so the number is
 *		representable.
 */
		exponent	= x + min_10_exp;
		absexp	= ( (exponent < 0) ? (-exponent) : exponent );

		absdd	/= d;
/*
 *		Step 2.  Now find the fraction part the hard way.
 */
		st[is++]	= ( (dd < 0.0) ? '-' : ' ');
		st[is++]	= '.';

		for( i = 0; i < m; ++i )
		{
			absdd	*= 10.0;			
			st[is++]	= (int) absdd + '0';
			absdd	-= (int) absdd;
		}
/*
 *		Step 3.  Append the exponent.
 */
		st[is++]	= 'E';
		st[is++]	= ( (exponent < 0) ? '-' : '+');

		if ( absexp >= 100 )
		{
			st[is++]	= absexp / 100 + '0';
			absexp	-= (absexp / 100) * 100;
			st[is++]	= absexp / 10 + '0';
			absexp	-= (absexp / 10) * 10;
			st[is++]	= absexp + '0';
		}
		else if ( absexp >= 10 )
		{
			st[is++]	= absexp / 10 + '0';
			absexp	-= (absexp / 10) * 10;
			st[is++]	= absexp + '0';
		}
		else
		{
			st[is++]	= absexp + '0';
		}
	}
	else
	{
/*
 *		The number is not representable.
 *
 *		This version cannot distinguish between Inf and NaN so
 *		put a suitable message in place.
 */
		is	= 0;

		if ( dd < 0.0 )
		{
			st[is++]	= '-';
		}

		st[is++]	= 'I'; st[is++]	= 'n'; st[is++]	= 'f';
		st[is++]	= ' ';
		st[is++]	= 'o'; st[is++]	= 'r';
		st[is++]	= ' ';
		st[is++]	= 'N'; st[is++]	= 'a'; st[is++]	= 'N';
	}

	st[is++]	= '\0';

	return st;
}
/*---------------------------------------------------------------*/
/*
 *	Convert a FLOAT to a string somehow.
 *
 *	Warning: This shares a statically allocated buffer.
 *	It could also overflow ca[] silently.
 */
char *
float2string( FLOAT f )
{
	static	char		ca[MAXCHAR];

#ifndef	NOSTDIO
#	ifdef	SINGLE
	sprintf( ca, "%.7e", f);
#	else
#		ifdef	LONG_DOUBLE
			sprintf( ca, "%.17Le", f);
#		else
			sprintf( ca, "%.17e", f);
#		endif
#	endif
#else
#	ifdef	SINGLE
	local_float2string(	f, 7, FLT_MIN_10_EXP, FLT_MAX_10_EXP,
					min_val, ca );
#	else
	local_float2string(	f, 17, DBL_MIN_10_EXP, DBL_MAX_10_EXP,
					min_val, ca );
#	endif
#endif

	return	ca;
}
/*---------------------------------------------------------------*/
/*
 *	Cheap and nasty way of producing a string from an int
 *	in case stdio.h is not available.  This will do for the integers
 *	printed out by ESP.
 *
 *	di			INPUT: integer to convert
 *	max_10_exp	INPUT: largest  allowable power of 10.
 *	st			OUTPUT: Output string.
 */
char *
local_int2string( long di, int max_10_exp, char * st )
{
	int		is		= 0;
	int		i;
	int		absdi;
	long		ldi		= 0;

	absdi	= ((di < 0) ? (-di) : (di) );

	if ( di != 0 )
	{
/*
 *		Walk up from zero to find a power of 10 bigger than our number.
 */
		ldi	= 1;

		for( i = 0; i < max_10_exp; ++i )
		{
			if ( ldi > (long) absdi )
			{
				break;
/*				======				*/
			}

			ldi	*= 10;
		}

		if ( i < max_10_exp )
		{
/*
 *			Successful.  ldi is a power of 10 bigger than our number.
 *			Now walk back down again picking up the digits.
 */
			for( ldi /= 10; ldi > 0; ldi /= 10 )
			{
				st[is++]	= (absdi/ldi) + '0';
				absdi	-= ((absdi/ldi) * ldi);
			}
		}
		else
		{
/*
 *			Not representable.
 */
			st[is++]	= 'N'; st[is++]	= 'o'; st[is++]	= 't';
			st[is++]	= ' ';
			st[is++]	= 'r'; st[is++]	= 'e'; st[is++]	= 'p';
			st[is++]	= 'r'; st[is++]	= 'e'; st[is++]	= 's';
			st[is++]	= 'e'; st[is++]	= 'n'; st[is++]	= 't';
			st[is++]	= 'a'; st[is++]	= 'b'; st[is++]	= 'l';
			st[is++]	= 'e';
		}
	}
	else
	{
/*
 *		The number is zero.
 */
		st[is++]	= '0';
	}

	st[is++]	= '\0';

	return	st;
}
/*---------------------------------------------------------------*/
/*
 *	Convert an integer to a string somehow.
 *	We cast to a long through the interface to make sure we
 *	can print reasonably large numbers in local_int2string().
 *
 *	Warning this shares a statically allocated buffer
 *	and uses sprintf for maximum compatibility but it could
 *	overflow ca silently.
 */
char *
int2string( long i )
{
	static	char		ca[MAXCHAR];

#ifndef	NOSTDIO
	sprintf( ca, "%ld", i);
#else
/*
 *	We are on our own.  Convert an integer to a string.
 */
	local_int2string( i, 9, ca );
#endif

	return	ca;
}
/*---------------------------------------------------------------*/
/*
 *	Something gone pear-shaped in the tests.
 */
void
notify(char *s)
{
	message( FALSE, "=======> Apparent inconsistency in test ");
	message( TRUE ,  s );
	message( TRUE , "=======> PLEASE NOTIFY lesh@oakcomp.co.uk.");
}
/*---------------------------------------------------------------*/
/*
 *	This pause does not actually pause.
 */
void
Pause(void)
{
	message( FALSE, "-------> Diagnosis resuming after Milestone ");
	message( FALSE,  int2string( Milestone ) );
	message( FALSE,  ", Page " );
	message( TRUE ,  int2string( PageNo ) );

	++Milestone;
	++PageNo;
}
/*---------------------------------------------------------------*/
/*
 *	fp exception handling.
 */
void
sigfpe(int dummy_param)
{
	fpecount++;
	message(TRUE, "FAILURE: * * * FLOATING-POINT ERROR * * *");

	if (sigsave)
	{
#ifndef NOSIGNAL
		signal(SIGFPE, sigsave);
#endif
		sigsave = 0;
#ifndef NOSETJMP
		longjmp(ovfl_buf, 1);
#endif
	}

	exit(EXIT_FAILURE);
}
/*---------------------------------------------------------------*/
void
BadCond( int K, char *T)
{
	static char *msg[] = { "FAILURE", "SDEFECT", "DEFECT ", "FLAW   " };

	ErrCnt[K] = ErrCnt[K] + 1;
	message( FALSE, msg[K] );
	message( FALSE, ": " );
	message( TRUE, T );
}
/*---------------------------------------------------------------*/
void
TstCond ( int K, int Valid, char *T)
{
	if (! Valid)
	{
		BadCond(K,T);
	}
}
/*---------------------------------------------------------------*/
void
TstPtUf(void)
{
	N = 0;
	if (Z != Zero)
	{
		message(TRUE, "COMMENT: Since comparison denies Z = 0, evaluating ");
		message(TRUE, "COMMENT: (Z + Z) / Z should be safe.");

		sigsave = sigfpe;

#ifndef	NOSETJMP
		if (setjmp(ovfl_buf)) goto very_serious;
#endif

		Q9 = (Z + Z) / Z;

		if (FABS(Q9 - Two) < Radix * U2)
		{
			message( FALSE, "COMMENT: (Z+Z)/Z is OK ");
#ifndef		NOSETJMP
			message( TRUE, "provided Over/Underflow");
			message( TRUE, "COMMENT: has NOT just been signaled.");
#else
			message( TRUE, "but this installation cannot");
			message( TRUE, "COMMENT: signal any Over/Underflow.");
#endif
		}
		else
		{
			if ((Q9 < One) || (Q9 > Two))
			{
very_serious:
				N = 1;
				ErrCnt [SERIOUS] = ErrCnt [SERIOUS] + 1;
				message(TRUE, "COMMENT: This is a VERY SERIOUS DEFECT !");
				}
			else
			{
				N = 1;
				ErrCnt [DEFECT] = ErrCnt [DEFECT] + 1;
				message(TRUE, "COMMENT: This is a DEFECT !");
			}
		}

		sigsave = 0;
		V9 = Z * One;
		Random1 = V9;
		V9 = One * Z;
		Random2 = V9;
		V9 = Z / One;

		if ((Z == Random1) && (Z == Random2) && (Z == V9))
		{
			if (N > 0) Pause();
		}
		else
		{
			N = 1;

			if ( Z != Random1 )
			{
				BadCond(DEFECT, "Z != Z * 1");

				message( FALSE, "COMMENT: Z = " );
				message( FALSE, float2string( Z ) );
				message( FALSE, ", Z * 1 = " );
				message( TRUE , float2string( Random1 ) );
			}

			if ( Z != Random2 )
			{
				BadCond(DEFECT, "Z != 1 * Z");

				message( FALSE, "COMMENT: Z = " );
				message( FALSE, float2string( Z ) );
				message( FALSE, ", 1 * Z = " );
				message( TRUE , float2string( Random2 ) );
			}

			if ( Z != V9 )
			{
				BadCond(DEFECT, "Z != Z / 1");

				message( FALSE, "COMMENT: Z = " );
				message( FALSE, float2string( Z ) );
				message( FALSE, ", Z / 1 = " );
				message( TRUE , float2string( V9 ) );
			}

			if (Random2 != Random1) {
				ErrCnt [DEFECT] = ErrCnt [DEFECT] + 1;
				BadCond(DEFECT, "Z * 1 != 1 * Z\n");
			}

			Pause();
		}
	}
}
/*---------------------------------------------------------------*/
void
SR3750(void)
{
	if (! ((X - Radix < Z2 - Radix) || (X - Z2 > W - Z2)))
	{
		I = I + 1;
		X2 = SQRT(X * D);
		Y2 = (X2 - Z2) - (Y - Z2);
		X2 = X8 / (Y - Half);
		X2 = X2 - Half * X2 * X2;
		SqEr = (Y2 + Half) + (Half - X2);

		if (SqEr < MinSqEr)
		{
			MinSqEr = SqEr;
		}

		SqEr = Y2 - X2;
		if (SqEr > MaxSqEr)
		{
			MaxSqEr = SqEr;
		}
	}
}
/*---------------------------------------------------------------*/
void
NewD()
{
	X = Z1 * Q;
	X = FLOOR(Half - X / Radix) * Radix + X;
	Q = (Q - X * Z) / Radix + X * X * (D / Radix);
	Z = Z - Two * X * D;

	if (Z <= Zero)
	{
		Z = - Z;
		Z1 = - Z1;
	}

	D = Radix * D;
}
/*---------------------------------------------------------------*/
#ifndef 	NOHYPERBOLIC
/*
 *	For these functions, we simply record the maximum error
 *	encountered.
 *
 *	(cosh(X)**2 - sinh(X)**2) - 1.0
 */
void
DiffHyper1(void)
{
	FLOAT XA, XB;

	XB	= COSH(X);
	XA	= SINH(X);
	SqEr	= (((XB*XB) - (XA*XA)) - One);

	if (SqEr != Zero)
	{
		if (SqEr < MinSqEr) MinSqEr = SqEr;
		if (SqEr > MaxSqEr) MaxSqEr = SqEr;
	}

	MINERR_DiffHyper1	= MinSqEr;
	MAXERR_DiffHyper1	= MaxSqEr;
}
#endif
/*---------------------------------------------------------------*/
#ifndef 	NOTRANSCENDENTAL
/*
 *	For these functions, we simply record the maximum error
 *	encountered.
 *
 *	(sin(X)**2 + cos(X)**2) - 1.0
 */
void
DiffTran1(void)
{
	FLOAT XA, XB;

	XB	= COS(X);
	XA	= SIN(X);
	SqEr	= (((XA*XA) + (XB*XB)) - One);

	if (SqEr != Zero)
	{
		if (SqEr < MinSqEr) MinSqEr = SqEr;
		if (SqEr > MaxSqEr) MaxSqEr = SqEr;
	}

	MINERR_DiffTran1	= MinSqEr;
	MAXERR_DiffTran1	= MaxSqEr;
}
/*---------------------------------------------------------------*/
/*
 *	Abramowitz and Stegun, 4.3.29, Handbook of mathematical functions.
 *
 *	sin(4X) - 8.0 * cos**4(X) * tan(X) + 4.0 * cos(X) * sin(X)
 */
void
DiffTran2 (void)
{
	FLOAT XA, XB, XC, XD;

	XB	= COS(X);
	XA	= SIN(X);
	XC	= TAN(X);
	XD	= SIN(Four * X);
	SqEr	= XD - (Eight * XB * XB * XB * XB * XC) + (Four * XB * XA);

	if (SqEr != Zero)
	{
		if (SqEr < MinSqEr) MinSqEr = SqEr;
		if (SqEr > MaxSqEr) MaxSqEr = SqEr;
	}

	MINERR_DiffTran2	= MinSqEr;
	MAXERR_DiffTran2	= MaxSqEr;
}
#endif
/*---------------------------------------------------------------*/
/*
 *	exp(log(X)) - X
 */
void
DiffExpLog(void)
{
	SqEr	= (EXP(LOG(X)) - X);
	if (SqEr != Zero)
	{
		if (SqEr < MinSqEr) MinSqEr = SqEr;
		if (SqEr > MaxSqEr) MaxSqEr = SqEr;
	}

	MINERR_DiffExpLog	= MinSqEr;
	MAXERR_DiffExpLog	= MaxSqEr;
}
/*---------------------------------------------------------------*/
/*
 *	(X**2 - Y**2) - (X-Y)(X+Y)
 */
void
Diff2Sq (int ErrKind)
{
	FLOAT XA, XB;

	XB	= X * BInvrse;
	XA	= X;
	SqEr	= (((XA*XA) - (XB*XB)) - (XA-XB) * (XA+XB)) / OneUlp;
	if (SqEr != Zero)
	{
		if (SqEr < MinSqEr) MinSqEr = SqEr;
		if (SqEr > MaxSqEr) MaxSqEr = SqEr;

		J = J + 1.0;
		BadCond(ErrKind, "\n");

		message( FALSE, "COMMENT: (X**2 - Y**2) - (X-Y)*(X+Y)" );
		message( FALSE, float2string( OneUlp * SqEr ) );
		message( TRUE, " rather than 0." );
	}
}
/*---------------------------------------------------------------*/
/*
 *	Square(X) - X.
 */
void
SqXMinX (int ErrKind)
{
	FLOAT XA, XB;
	
	XB = X * BInvrse;
	XA = X - XB;
	SqEr = ((SQRT(X * X) - XB) - XA) / OneUlp;
	if (SqEr != Zero)
	{
		if (SqEr < MinSqEr) MinSqEr = SqEr;
		if (SqEr > MaxSqEr) MaxSqEr = SqEr;

		J = J + 1.0;
		BadCond(ErrKind, "\n");

		message( FALSE, "COMMENT: sqrt( X * X ) - X = " );
		message( FALSE, float2string( OneUlp * SqEr ) );
		message( TRUE, " rather than 0." );
	}
}
/*---------------------------------------------------------------*/
void
PrintIfNPositive()
{
	if (N > 0)
	{
		message( FALSE, "COMMENT: Number of similar discrepancies = " );
		message( TRUE , int2string( N ) );
	}
}
/*---------------------------------------------------------------*/
void
IsYeqX()
{
	if (Y != X)
	{
		if (N <= 0)
		{
			if (	(Z == Zero) && (Q <= Zero) )
			{
				message( TRUE, "COMMENT: Power function ");
			}
			else
			{
				BadCond(DEFECT, "Power function ");
			}

			message( FALSE, "COMMENT: " );
			message( FALSE, float2string( Z ) );
			message( FALSE, " ^ " );
			message( FALSE, float2string( Q ) );
			message( FALSE, " = " );
			message( TRUE , float2string( Y ) );
			message( FALSE, "COMMENT: which differs from correct value by ");
			message( TRUE , float2string( Y - X ) );
		}

		N = N + 1; /* ... count discrepancies. */
	}
}

/*---------------------------------------------------------------*/
void
SR3980(void)
{
	do
	{
		Q = (FLOAT) I;
		Y = POW(Z, Q);
		IsYeqX();
		if (++I > M) break;
		X = Z * X;
	} while ( X < W );
}
/*---------------------------------------------------------------*/
/*
 *	Random computes
 *	X = (Random1 + Random9)^5
 *	Random1 = X - FLOOR(X) + 0.000005 * X;
 *	and returns the new value of Random1
 */
FLOAT
Random(void)
{
	FLOAT X, Y;
	
	X = Random1 + Random9;
	Y = X * X;
	Y = Y * Y;
	X = X * Y;
	Y = X - FLOOR(X);
	Random1 = Y + X * 0.000005;
	return(Random1);
}
/*---------------------------------------------------------------*/
/*
 *	Return +/- 1.0 depending on the sign of X.
 */
FLOAT
Sign (FLOAT X)
{
	return (X >= 0.) ? 1.0 : -1.0;
}
/*---------------------------------------------------------------*/
void
otherScores( void )
{
	message( TRUE  , "COMMENT:" );
	message( TRUE  , "COMMENT: -----------------------------------------" );
	message( TRUE  , "COMMENT:           MATHS LIBRARY TESTS" );
	message( TRUE  , "COMMENT:         (should all give zero)" );
	message( TRUE  , "COMMENT:" );
	message( TRUE  , "COMMENT: Basic identities" );
	message( TRUE  , "COMMENT: ln(1.0) = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( LOG(One) ) );
	message( TRUE  , "COMMENT: exp(1.0) - 2.718281828459045235360287 = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( EXP(One) - MATH_E ) );

	message( TRUE  , "COMMENT: Basic tests (Random over range 0<=X<=1)" );
	message( FALSE , "COMMENT:" );
	message( TRUE  , " Test, exp(log(X)) - X = "  );
	message( FALSE , "COMMENT:     " );
	message( FALSE , float2string( MINERR_DiffExpLog ) );
	message( FALSE , " -> " );
	message( TRUE  , float2string( MAXERR_DiffExpLog ) );
#ifndef 	NOTRANSCENDENTAL
	message( TRUE  , "COMMENT:" );
	message( TRUE  , "COMMENT: Transcendental identities" );
	message( TRUE  , "COMMENT: Test, sin(1.0) - 0.84147098480789650665250 = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( SIN(One) - SIN_ONE ) );
	message( TRUE  , "COMMENT: Test, cos(1.0) - 0.54030230586813971740094 = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( COS(One) - COS_ONE ) );
	message( TRUE  , "COMMENT: Test, asin(1.0) - (pi/2) = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( ASIN(One) - (MATH_PI/Two) ) );
	message( TRUE  , "COMMENT: Test, acos(0.0) - (pi/2) = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( ACOS(0.0) - (MATH_PI/Two) ) );
	message( TRUE  , "COMMENT: Test, atan(1.0) - (pi/4) = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( ATAN(One) - (MATH_PI/Four) ) );

	message( TRUE  , "COMMENT: Transcendental tests (Random over range 0<=X<=1)" );
	message( FALSE , "COMMENT:" );
	message( TRUE  , " Test, sin**2(X)+cos**2(X)-1.0 = "  );
	message( FALSE , "COMMENT:     " );
	message( FALSE , float2string( MINERR_DiffTran1 ) );
	message( FALSE , " -> " );
	message( TRUE  , float2string( MAXERR_DiffTran1 ) );
	message( FALSE , "COMMENT:" );
	message( TRUE  , " Test, sin(4X)-(8.0*cos**4(X)*tan(X))+(4.0*cos(X)*sin(X)) = "  );
	message( FALSE , "COMMENT:     " );
	message( FALSE , float2string( MINERR_DiffTran2 ) );
	message( FALSE , " -> " );
	message( TRUE  , float2string( MAXERR_DiffTran2 ) );
#endif
#ifndef 	NOHYPERBOLIC
	message( TRUE  , "COMMENT:" );
	message( TRUE  , "COMMENT: Hyperbolic identities" );
	message( TRUE  , "COMMENT: Test, sinh(0.0)  = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( SINH(0.0) ) );
	message( TRUE  , "COMMENT: Test, cosh(0.0) - 1.0  = " );
	message( FALSE , "COMMENT:     " );
	message( TRUE  , float2string( COSH(0.0) - One ) );

	message( TRUE  , "COMMENT: Hyperbolic tests (Random over range 0<=X<=1)" );
	message( FALSE , "COMMENT:" );
	message( TRUE  , " Test, cosh**2(X)-sinh**2(X)-1.0 = "  );
	message( FALSE , "COMMENT:     " );
	message( FALSE , float2string( MINERR_DiffHyper1 ) );
	message( FALSE , " -> " );
	message( TRUE  , float2string( MAXERR_DiffHyper1 ) );
#endif
}
/*---------------------------------------------------------------*/
void
finalScore( int identify, char *string )
{
	message( FALSE , "COMMENT:" );

	if ( identify )
	{
		message( FALSE, "         =====> "  );
	}
	else
	{
		message( FALSE, "                "  );
	}

	message( TRUE, string );
}
/*---------------------------------------------------------------*/
/*
 *	Set up the constants for integer and float to string conversion.
 */
void
set_conversion_constants(void)
{
#ifdef	SINGLE
#	define	MIN_10_EXP	FLT_MIN_10_EXP
#else
#	define	MIN_10_EXP	DBL_MIN_10_EXP
#endif
	min_val	= 1.0;

	for( I = 0; I > MIN_10_EXP; --I )
	{
		min_val	/= 10.0;		
	}
}
/*---------------------------------------------------------------*/
int
main(void)
{
	set_conversion_constants();
/*
 *	Now test if the setjmp() facility is working.
 */
#ifdef	TESTSETJMP
	if (setjmp(ovfl_buf))
	{
		message( TRUE , "COMMENT: setjmp return from longjmp() correctly." );
		exit(EXIT_SUCCESS);
	}

	longjmp(ovfl_buf, 1);
#endif
/*
 *	Set the main constants.
 */
	Zero 			= 0;		/*	Deliberately integer	*/
	One 				= 1;		/*	Deliberately integer	*/

	Two 				= One + One;
	Three 			= Two + One;
	Four 			= Three + One;
	Five 			= Four + One;
	Eight 			= Four + Four;
	Nine 			= Three * Three;
	TwentySeven 		= Nine * Three;
	ThirtyTwo 		= Four * Eight;
	TwoForty 			= Four * Five * Three * Four;
	MinusOne 			= -One;
	Half 			= One / Two;
	OneAndHalf 		= One + Half;
/*
 *	Set the basic error counters for each type.
 */
	ErrCnt[FAILURE] 	= 0;
	ErrCnt[SERIOUS] 	= 0;
	ErrCnt[DEFECT] 	= 0;
	ErrCnt[FLAW] 		= 0;

	PageNo 			= 1;
	message( TRUE , "COMMENT: =========================================" );
	message( TRUE , "COMMENT: Welcome to ESP - Embedded System Paranoia" );
	message( TRUE , "COMMENT: Please let me know your experiences" );
	message( TRUE , "COMMENT: and suggestions at lesh@oakcomp.co.uk or" );
	message( TRUE , "COMMENT: L.Hatton@kent.ac.uk" );
	message( TRUE , "COMMENT: " );
	message( TRUE , "COMMENT: $Revision: 1.1 $ $Date: 2019/12/12 14:19:23 $" );
#ifndef	NOZERODIVIDE
	message( TRUE , "COMMENT: This version will attempt divide by zero." );
#endif
#ifndef	NOSTDIO
	message( TRUE , "COMMENT: This version uses <stdio.h>" );
#else
	message( TRUE , "COMMENT: This version uses internal numeric " );
	message( TRUE , "COMMENT: conversion functions which cannot " );
	message( TRUE , "COMMENT: distinguish Inf and Nan." );
#endif
#ifndef	NOSIGNAL
	message( TRUE , "COMMENT: This version uses <signal.h>" );
#endif
#ifndef	NOSETJMP
	message( TRUE , "COMMENT: This version uses <setjmp.h>" );
#endif
#ifdef	SINGLE
	message( FALSE , "COMMENT: This version uses single precision, nbits=" );
#else
#ifdef  LONG_DOUBLE
	message( FALSE , "COMMENT: This version uses long double precision, nbits=" );
#else
	message( FALSE , "COMMENT: This version uses double precision. nbits=" );
#endif
#endif
	message( TRUE , int2string( sizeof(FLOAT)*8 ) );
	message( TRUE , "COMMENT: =========================================" );
	/*=============================================*/
	Milestone = 0;
	/*=============================================*/
#ifndef NOSIGNAL
	signal(SIGFPE, sigfpe);
#endif
	if ( (Zero + Zero == Zero)		&&
		(One - One == Zero)			&&
		(One > Zero)				&&
		(One + One == Two)				)
	{
/*
 *		Its OK.
 */
	}
	else
	{
		message( TRUE, "FAILURE: 0+0 != 0, 1-1 != 0, 1 <= 0, or 1+1 != 2" );
	}

	Z = - Zero;

	if (Z != 0.0)
	{
		ErrCnt[FAILURE] = ErrCnt[FAILURE] + 1;
		message( TRUE, "FAILURE: Comparison says -0.0 is non-zero !" );
		U1 = 0.001;
		Radix = 1;
		TstPtUf();
	}

	TstCond (FAILURE, (Three == Two + One) && (Four == Three + One)
		   && (Four + Two * (- Two) == Zero)
		   && (Four - Three - One == Zero),
		   "3 != 2+1, 4 != 3+1, 4+2*(-2) != 0, or 4-3-1 != 0");
	TstCond (FAILURE, (MinusOne == (0 - One))
		   && (MinusOne + One == Zero ) && (One + MinusOne == Zero)
		   && (MinusOne + FABS(One) == Zero)
		   && (MinusOne + MinusOne * MinusOne == Zero),
		   "-1+1 != 0, (-1)+abs(1) != 0, or -1+(-1)*(-1) != 0");
	TstCond (FAILURE, Half + MinusOne + Half == Zero,
		  "1/2 + (-1) + 1/2 != 0");
	Pause();
	/*=============================================*/
	Milestone = 10;
	/*=============================================*/
	TstCond (FAILURE, (Nine == Three * Three)
		   && (TwentySeven == Nine * Three) && (Eight == Four + Four)
		   && (ThirtyTwo == Eight * Four)
		   && (ThirtyTwo - TwentySeven - Four - One == Zero),
		   "9 != 3*3, 27 != 9*3, 32 != 8*4, or 32-27-4-1 != 0");
	TstCond (FAILURE, (Five == Four + One) &&
			(TwoForty == Four * Five * Three * Four)
		   && (TwoForty / Three - Four * Four * Five == Zero)
		   && ( TwoForty / Four - Five * Three * Four == Zero)
		   && ( TwoForty / Five - Four * Three * Four == Zero),
		  "5 != 4+1, 240/3 != 80, 240/4 != 60, or 240/5 != 48");

	if (ErrCnt[FAILURE] == 0)
	{
		message( FALSE, "COMMENT: ");
		message( TRUE , "-1, 0, 1/2, 1, 2, 3, 4, 5, 9, 27, 32 & 240");
		message( TRUE , "PASSED : small integer tests are all OK.");
	}

	message( TRUE, "COMMENT: Searching for Radix and Precision.");

	W = One;
	do
	{
		W = W + W;
		Y = W + One;
		Z = Y - W;
		Y = Z - One;
	} while (MinusOne + FABS(Y) < Zero);

	/*.. now W is just big enough that |((W+1)-W)-1| >= 1 ...*/

	Precision = Zero;
	Y = One;

	do
	{
		Radix = W + Y;
		Y = Y + Y;
		Radix = Radix - W;
	} while ( Radix == Zero);

	if (Radix < Two) Radix = One;
		
	message( FALSE, "COMMENT: Radix = " );  
	message( TRUE , float2string( Radix ) );

	if (Radix != 1)
	{
		W = One;
		do
		{
			Precision = Precision + One;
			W = W * Radix;
			Y = W + One;
		} while ((Y - W) == One);
	}

	/*... now W == Radix^Precision is barely too big to satisfy (W+1)-W == 1
			                              ...*/
/* 
 *	NOTE: Computed constants ...
 *
 *		U1 is the gap below 1.0, so 1.0 - U1 is the next number below
 *		1.0.
 *
 *		U2 is the gap above 1.0, so 1.0 + U2 is the next number above
 *		1.0.
 */
	U1 = One / W;
	U2 = Radix * U1;

	message( FALSE, "COMMENT: Closest relative separation found is U1 = " );
	message( TRUE , float2string( U1 ) );

	message( TRUE , "COMMENT: Recalculating radix and precision" );
	
	/*save old values*/
	E0 = Radix;
	E1 = U1;
	E9 = U2;
	E3 = Precision;
	
	X = Four / Three;
	Third = X - One;
	F6 = Half - Third;
	X = F6 + F6;
	X = FABS(X - Third);
	if (X < U2) X = U2;
	
	/*... now X = (unknown no.) ulps of 1+...*/
	do
	{
		U2 = X;
		Y = Half * U2 + ThirtyTwo * U2 * U2;
		Y = One + Y;
		X = Y - One;
	} while ( ! ((U2 <= X) || (X <= Zero)));
	
	/*... now U2 == 1 ulp of 1 + ... */

	X = Two / Three;
	F6 = X - Half;
	Third = F6 + F6;
	X = Third - Half;
	X = FABS(X + F6);
	if (X < U1) X = U1;
	
	/*... now  X == (unknown no.) ulps of 1 -... */
	do
	{
		U1 = X;
		Y = Half * U1 + ThirtyTwo * U1 * U1;
		Y = Half - Y;
		X = Half + Y;
		Y = Half - X;
		X = Half + Y;
	} while ( ! ((U1 <= X) || (X <= Zero)));

	/*... now U1 == 1 ulp of 1 - ... */

	if (U1 == E1)
	{
		message( TRUE ,"COMMENT: confirms closest relative separation U1.");
	}
	else
	{
		message( FALSE,
			"COMMENT: gets better closest relative separation U1 =" );
		message( TRUE , float2string( U1 ) );
	}

	/* Sanity check - compare calculated (U1) and supplied epsilons */
	EpsilonOverU1 = EPSILON / U1;
	message( TRUE,
		"COMMENT: Checking U1 for sanity..." );
	if (EpsilonOverU1 > 2.0 || EpsilonOverU1 < 0.5)
	{
	  message( TRUE,
		  "WARNING: Serious discrepancy between calculated U1 and EPSILON" );
	  message( TRUE,
		  "WARNING: and that supplied in float.h for the floating type used" );
	  message( FALSE,
		  "COMMENT: Supplied EPSILON is: " );
	  message( TRUE, float2string( EPSILON ));
	  message( TRUE,
		  "COMMENT: This is probably because the default working size of a float on" );
	  message( TRUE,
		  "COMMENT: this machine is much larger that the float type under test." );
	  message( TRUE,
		  "WARNING: FURTHER TEST RESULTS AND RATING ARE LIKELY TO BE UNRELIABLE !!!" );
	}
	else
	{
	  message( TRUE,
		  "COMMENT: U1 is a sensible value" );
	}

	W = One / U1;
	F9 = (Half - U1) + Half;
	Radix = FLOOR(0.01 + U2 / U1);
	if (Radix == E0)
	{
		message( TRUE ,"COMMENT: Radix confirmed.");
	}
	else
	{
		message( FALSE,
			"MYSTERY: recalculated Radix =" );
		message( TRUE , float2string( Radix ) );
	}

	TstCond (DEFECT, Radix <= Eight + Eight,
		   "Radix is too big: roundoff problems");
	TstCond (FLAW, (Radix == Two) || (Radix == 10)
		   || (Radix == One), "Radix is not as good as 2 or 10");

	Pause();
	/*=============================================*/
	Milestone = 20;
	/*=============================================*/
	TstCond (FAILURE, F9 - Half < Half,
		   "(1-U1)-1/2 < 1/2 is FALSE, prog. fails?");
	X = F9;
	I = 1;
	Y = X - Half;
	Z = Y - Half;
	TstCond (FAILURE, (X != One)
		   || (Z == Zero), "Comparison is fuzzy,X=1 but X-1/2-1/2 != 0");
	X = One + U2;
	I = 0;

	Pause();
	/*=============================================*/
	Milestone = 25;
	/*=============================================*/
	/*... BMinusU2 = nextafter(Radix, 0) */
	BMinusU2 = Radix - One;
	BMinusU2 = (BMinusU2 - U2) + One;

	/* Purify Integers */
	if (Radix != One)
	{
		X = - TwoForty * LOG(U1) / LOG(Radix);
		Y = FLOOR(Half + X);
		if (FABS(X - Y) * Four < One) X = Y;
		Precision = X / TwoForty;
		Y = FLOOR(Half + Precision);
		if (FABS(Precision - Y) * TwoForty < Half) Precision = Y;
	}

	if ((Precision != FLOOR(Precision)) || (Radix == One))
	{
		message( TRUE,
		"COMMENT: Precision cannot be characterized by an Integer number");
		message( TRUE ,
		"COMMENT: of significant digits but, by itself, this is ");
		message( TRUE ,
		"COMMENT: a minor flaw.");
	}

	if (Radix == One) 
	{
		message( TRUE ,
		"COMMENT: logarithmic encoding has precision characterized ");
		message( TRUE ,
		"COMMENT: solely by U1.");
	}
	else
	{
		message( TRUE , "COMMENT: The number of significant digits of the ");
		message( FALSE, "COMMENT: Radix is ");
		message( TRUE, float2string( Precision ) );
	}

	TstCond (SERIOUS, U2 * Nine * Nine * TwoForty < One,
		   "Precision worse than 5 decimal figures  ");

	Pause();
	/*=============================================*/
	Milestone = 30;
	/*=============================================*/
	/* Test for extra-precise subexpressions */
	X = FABS(((Four / Three - One) - One / Four) * Three - One / Four);
	do
	{
		Z2 = X;
		X = (One + (Half * Z2 + ThirtyTwo * Z2 * Z2)) - One;
	} while ( ! ((Z2 <= X) || (X <= Zero)));

	X = Y = Z = FABS((Three / Four - Two / Three) * Three - One / Four);

	do
	{
		Z1 = Z;
		Z = (One / Two - ((One / Two - (Half * Z1 + ThirtyTwo * Z1 * Z1))
			+ One / Two)) + One / Two;
	} while ( ! ((Z1 <= Z) || (Z <= Zero)));

	do
	{
		do
		{
			Y1 = Y;
			Y = (Half - ((Half - (Half * Y1 + ThirtyTwo * Y1 * Y1)) + Half
				)) + Half;
		} while ( ! ((Y1 <= Y) || (Y <= Zero)));

		X1 = X;
		X = ((Half * X1 + ThirtyTwo * X1 * X1) - F9) + F9;
	} while ( ! ((X1 <= X) || (X <= Zero)));

	if ((X1 != Y1) || (X1 != Z1))
	{
		BadCond(SERIOUS, "Disagreements among the values X1, Y1, Z1.\n");
		message( FALSE, "COMMENT: X1 = " );
		message( FALSE, float2string( X1 ) );
		message( FALSE, ", Y1= " );
		message( FALSE, float2string( Y1 ) );
		message( FALSE, ", Z1= " );
		message( TRUE , float2string( Z1 ) );
		message( TRUE ,"COMMENT: This is a symptom of inconsistencies");
		message( TRUE ,"COMMENT: introduced by extra-precise evaluation");
		message( TRUE ,"COMMENT: of arithmetic subexpressions.");

		notify("X1, Y1, Z1 or some part of this.");

		if ((X1 == U1) || (Y1 == U1) || (Z1 == U1))
		{
			message( TRUE,
			"COMMENT: This is not tested further by this program.\n") ;
		}
	}
	else
	{
		if ((Z1 != U1) || (Z2 != U2))
		{
			if ((Z1 >= U1) || (Z2 >= U2))
			{
				BadCond(FAILURE, "");
				notify("Precision");
				message( FALSE, "COMMENT: U1 = " );
				message( FALSE, float2string( U1 ) );
				message( FALSE, ", Z1 - U1 = " );
				message( TRUE , float2string( Z1 - U1 ) );
				message( FALSE, "COMMENT: U2 = " );
				message( FALSE, float2string( U2 ) );
				message( FALSE, ", Z2 - U2 = " );
				message( TRUE , float2string( Z2 - U2 ) );
			}
			else
			{
				if ((Z1 <= Zero) || (Z2 <= Zero))
				{
					message( FALSE, "COMMENT: Z = " );
					message( FALSE, float2string( Z ) );
					message( FALSE, ", Z / 1 = " );
					message( TRUE , float2string( V9 ) );

					message( FALSE, "COMMENT: Unusual Radix = " );
					message( FALSE, float2string( Radix ) );
					message( TRUE , "or exact rational arithmetic" );
					message( FALSE, "COMMENT: gives Z1 = " );
					message( FALSE, float2string( Z1 ) );
					message( FALSE, ", Z2 = " );
					message( TRUE , float2string( Z2 ) );

					notify("of an\nextra-precision");
				}

				if (Z1 != Z2 || Z1 > Zero)
				{
					X = Z1 / U1;
					Y = Z2 / U2;

					if (Y > X)
					{
						X = Y;
					}

					Q = - LOG(X);

					message( FALSE,
					"COMMENT: Some subexpressions appear to be " );
					message( TRUE , "calculated extra precisely with" );

					message( FALSE, "COMMENT: about " );
					message( FALSE, float2string( Q / LOG(10.) ) );
					message( TRUE , " extra significant decimals." );
				}

				message( TRUE,
				"COMMENT: This is not tested further by this program." );
			}
		}
	}

	Pause();
	/*=============================================*/
	Milestone = 35;
	/*=============================================*/
	if (Radix >= Two)
	{
		X = W / (Radix * Radix);
		Y = X + One;
		Z = Y - X;
		T = Z + U2;
		X = T - Z;
		TstCond (FAILURE, X == U2,
			"Subtraction is not normalized X=Y,X+Z != Y+Z!");
		if (X == U2)
		{
			message( TRUE,
			"COMMENT: Subtraction appears to be normalized, as it should be.");
		}
	}

	message( TRUE,
		"COMMENT: Checking for guard digit in *, / and -.");

	Y = F9 * One;
	Z = One * F9;
	X = F9 - Half;
	Y = (Y - Half) - X;
	Z = (Z - Half) - X;
	X = One + U2;
	T = X * Radix;
	R = Radix * X;
	X = T - Radix;
	X = X - Radix * U2;
	T = R - Radix;
	T = T - Radix * U2;
	X = X * (Radix - One);
	T = T * (Radix - One);

	if ((X == Zero) && (Y == Zero) && (Z == Zero) && (T == Zero))
	{
		GMult = YES;
	}
	else
	{
		GMult = NO;
		TstCond (SERIOUS, FALSE,
			"* lacks a Guard Digit, so 1*X != X");
	}

	Z = Radix * U2;
	X = One + Z;
	Y = FABS((X + Z) - X * X) - U2;
	X = One - U2;
	Z = FABS((X - U2) - X * X) - U1;

	TstCond (FAILURE, (Y <= Zero)
		   && (Z <= Zero), "* gets too many final digits wrong.\n");

	Y = One - U2;
	X = One + U2;
	Z = One / Y;
	Y = Z - X;
	X = One / Three;
	Z = Three / Nine;
	X = X - Z;
	T = Nine / TwentySeven;
	Z = Z - T;

	TstCond(DEFECT, X == Zero && Y == Zero && Z == Zero,
		"Division lacks a Guard Digit, so error can exceed 1 ulp\n\
or  1/3  and  3/9  and  9/27 may disagree");

	Y = F9 / One;
	X = F9 - Half;
	Y = (Y - Half) - X;
	X = One + U2;
	T = X / One;
	X = T - X;

	if ((X == Zero) && (Y == Zero) && (Z == Zero))
	{
		GDiv = YES;
	}
	else
	{
		GDiv = NO;
		TstCond (SERIOUS, FALSE,
			"Division lacks a Guard Digit, so X/1 != X");
	}

	X = One / (One + U2);
	Y = X - Half - Half;

	TstCond (SERIOUS, Y < Zero,
		   "Computed value of 1/1.000..1 >= 1");

	X = One - U2;
	Y = One + Radix * U2;
	Z = X * Radix;
	T = Y * Radix;
	R = Z / Radix;
	StickyBit = T / Radix;
	X = R - X;
	Y = StickyBit - Y;

	TstCond (FAILURE, X == Zero && Y == Zero,
			"* and/or / gets too many last digits wrong");

	Y = One - U1;
	X = One - F9;
	Y = One - Y;
	T = Radix - U2;
	Z = Radix - BMinusU2;
	T = Radix - T;

	if ((X == U1) && (Y == U1) && (Z == U2) && (T == U2))
	{
		GAddSub = YES;
	}
	else
	{
		GAddSub = NO;
		TstCond (SERIOUS, FALSE,
			"- lacks Guard Digit, so cancellation is obscured");
	}

	if ( (F9 != One) && ((F9 - One) >= Zero) )
	{
		BadCond(SERIOUS, "Comparison alleges  (1-U1) < 1  although\n");
		message( TRUE,
			"COMMENT: subtraction yields  (1-U1) - 1 = 0");
		message( TRUE,
			"COMMENT: This means that such precautions against division");
		message( TRUE,
			"COMMENT: by zero as ");
		message( TRUE,
			"COMMENT:  ...  if (X == 1.0) {.....} else {.../(X-1.0)...}");
		message( TRUE,
			"COMMENT: will not work.");
	}

	if (GMult == YES && GDiv == YES && GAddSub == YES)
	{
		message( TRUE,
		"PASSED : *, /, and - appear to have guard digits, as they should.");
	}

	Pause();
	/*=============================================*/
	Milestone = 40;
	/*=============================================*/
	message( TRUE,
		"COMMENT: Checking rounding on multiply, divide and add/subtract.");

	RMult = OTHER;
	RDiv = OTHER;
	RAddSub = OTHER;
	RadixD2 = Radix / Two;
	A1 = Two;
	Done = FALSE;

	do
	{
		AInvrse = Radix;
		do
		{
			X = AInvrse;
			AInvrse = AInvrse / A1;
		} while ( ! (FLOOR(AInvrse) != AInvrse));

		Done = (X == One) || (A1 > Three);
		if (! Done)
		{
			A1 = Nine + One;
		}
	} while ( ! (Done));

	if (X == One)
	{
		A1 = Radix;
	}

	AInvrse = One / A1;
	X = A1;
	Y = AInvrse;
	Done = FALSE;

	do
	{
		Z = X * Y - Half;
		TstCond (FAILURE, Z == Half,
			"X * (1/X) differs from 1");
		Done = X == Radix;
		X = Radix;
		Y = One / X;
	} while ( ! (Done));

	Y2 = One + U2;
	Y1 = One - U2;
	X = OneAndHalf - U2;
	Y = OneAndHalf + U2;
	Z = (X - U2) * Y2;
	T = Y * Y1;
	Z = Z - X;
	T = T - X;
	X = X * Y2;
	Y = (Y + U2) * Y1;
	X = X - OneAndHalf;
	Y = Y - OneAndHalf;
	if ((X == Zero) && (Y == Zero) && (Z == Zero) && (T <= Zero))
	{
		X = (OneAndHalf + U2) * Y2;
		Y = OneAndHalf - U2 - U2;
		Z = OneAndHalf + U2 + U2;
		T = (OneAndHalf - U2) * Y1;
		X = X - (Z + U2);
		StickyBit = Y * Y1;
		S = Z * Y2;
		T = T - Y;
		Y = (U2 - Y) + StickyBit;
		Z = S - (Z + U2 + U2);
		StickyBit = (Y2 + U2) * Y1;
		Y1 = Y2 * Y1;
		StickyBit = StickyBit - Y2;
		Y1 = Y1 - Half;

		if ((X == Zero) && (Y == Zero) && (Z == Zero) && (T == Zero)
			&& ( StickyBit == Zero) && (Y1 == Half))
		{
			RMult = ROUNDED;
			message( TRUE,
				"PASSED : Multiplication appears to round correctly.");
		}
		else	if ((X + U2 == Zero) && (Y < Zero) && (Z + U2 == Zero)
				&& (T < Zero) && (StickyBit + U2 == Zero)
				&& (Y1 < Half))
		{
				RMult = CHOPPED;
				message( TRUE,
					"PASSED : Multiplication appears to chop.");
		}
		else
		{
			ErrCnt [FLAW] = ErrCnt [FLAW] + 1;
			message( FALSE,
				"FLAW   : Multiplication neither chopped nor");
			message( TRUE , " correctly rounded.");
		}

		if ((RMult == ROUNDED) && (GMult == NO))
		{
			notify("Multiplication");
		}
	}
	else
	{
		ErrCnt [FLAW] = ErrCnt [FLAW] + 1;
		message( FALSE,
			"FLAW   : Multiplication neither chopped nor");
		message( TRUE , " correctly rounded.");
	}

	Pause();
	/*=============================================*/
	Milestone = 45;
	/*=============================================*/
	Y2 = One + U2;
	Y1 = One - U2;
	Z = OneAndHalf + U2 + U2;
	X = Z / Y2;
	T = OneAndHalf - U2 - U2;
	Y = (T - U2) / Y1;
	Z = (Z + U2) / Y2;
	X = X - OneAndHalf;
	Y = Y - T;
	T = T / Y1;
	Z = Z - (OneAndHalf + U2);
	T = (U2 - OneAndHalf) + T;

	if (! ((X > Zero) || (Y > Zero) || (Z > Zero) || (T > Zero)))
	{
		X = OneAndHalf / Y2;
		Y = OneAndHalf - U2;
		Z = OneAndHalf + U2;
		X = X - Y;
		T = OneAndHalf / Y1;
		Y = Y / Y1;
		T = T - (Z + U2);
		Y = Y - Z;
		Z = Z / Y2;
		Y1 = (Y2 + U2) / Y2;
		Z = Z - OneAndHalf;
		Y2 = Y1 - Y2;
		Y1 = (F9 - U1) / F9;

		if ((X == Zero) && (Y == Zero) && (Z == Zero) && (T == Zero)
			&& (Y2 == Zero) && (Y2 == Zero)
			&& (Y1 - Half == F9 - Half ))
		{
			RDiv = ROUNDED;
			message( TRUE,
				"PASSED : Division appears to round correctly.");
			if (GDiv == NO)
			{
				notify("Division");
			}
		}
		else if ((X < Zero) && (Y < Zero) && (Z < Zero) && (T < Zero)
			&& (Y2 < Zero) && (Y1 - Half < F9 - Half))
		{
			RDiv = CHOPPED;
			message( TRUE,
				"PASSED : Division appears to chop.");
		}
	}

	if (RDiv == OTHER)
	{
		ErrCnt [FLAW] = ErrCnt [FLAW] + 1;
		message( FALSE,
			"FLAW   : Division neither chopped nor");
		message( TRUE , " correctly rounded.");
	}

	BInvrse = One / Radix;
	TstCond (FAILURE, (BInvrse * Radix - Half == Half),
		   "Radix * ( 1 / Radix ) differs from 1");
	Pause();
	/*=============================================*/
	Milestone = 50;
	/*=============================================*/
	TstCond (FAILURE, ((F9 + U1) - Half == Half)
		   && ((BMinusU2 + U2 ) - One == Radix - One),
		   "Incomplete carry-propagation in Addition");
	X = One - U1 * U1;
	Y = One + U2 * (One - U2);
	Z = F9 - Half;
	X = (X - Half) - Z;
	Y = Y - One;

	if ((X == Zero) && (Y == Zero))
	{
		RAddSub = CHOPPED;
		message( TRUE,
			"PASSED : Add/Subtract appears to be chopped.");
	}

	if (GAddSub == YES)
	{
		X = (Half + U2) * U2;
		Y = (Half - U2) * U2;
		X = One + X;
		Y = One + Y;
		X = (One + U2) - X;
		Y = One - Y;
		if ((X == Zero) && (Y == Zero))
		{
			X = (Half + U2) * U1;
			Y = (Half - U2) * U1;
			X = One - X;
			Y = One - Y;
			X = F9 - X;
			Y = One - Y;

			if ((X == Zero) && (Y == Zero))
			{
				RAddSub = ROUNDED;
				message( FALSE,
					"PASSED : Addition/Subtraction appears to round ");
				message( TRUE , "correctly.");
			}
			else
			{
				message( FALSE,
					"COMMENT: Addition/Subtraction neither rounds ");
				message( TRUE , "nor chops.");
			}
		}
		else
		{
			message( FALSE,
				"COMMENT: Addition/Subtraction neither rounds ");
			message( TRUE , " nor chops.");
		}
	}
	else
	{
		message( FALSE,
			"COMMENT: Addition/Subtraction neither rounds ");
		message( TRUE , " nor chops.");
	}

	S = One;
	X = One + Half * (One + Half);
	Y = (One + U2) * Half;
	Z = X - Y;
	T = Y - X;
	StickyBit = Z + T;

	if (StickyBit != Zero)
	{
		S = Zero;
		BadCond(FLAW, "(X - Y) + (Y - X) is non zero!\n");
	}

	StickyBit = Zero;
	if ((GMult == YES) && (GDiv == YES) && (GAddSub == YES)
		&& (RMult == ROUNDED) && (RDiv == ROUNDED)
		&& (RAddSub == ROUNDED) && (FLOOR(RadixD2) == RadixD2))
	{
		message( TRUE, "COMMENT: Checking for sticky bit.\n");

		X = (Half + U1) * U2;
		Y = Half * U2;
		Z = One + Y;
		T = One + X;
		if ((Z - One <= Zero) && (T - One >= U2))
		{
			Z = T + Y;
			Y = Z - X;

			if ((Z - T >= U2) && (Y - T == Zero))
			{
				X = (Half + U1) * U1;
				Y = Half * U1;
				Z = One - Y;
				T = One - X;

				if ((Z - One == Zero) && (T - F9 == Zero))
				{
					Z = (Half - U1) * U1;
					T = F9 - Z;
					Q = F9 - Y;

					if ((T - F9 == Zero) && (F9 - U1 - Q == Zero))
					{
						Z = (One + U2) * OneAndHalf;
						T = (OneAndHalf + U2) - Z + U2;
						X = One + Half / Radix;
						Y = One + Radix * U2;
						Z = X * Y;

						if (T == Zero && X + Radix * U2 - Z == Zero)
						{
							if (Radix != Two)
							{
								X = Two + U2;
								Y = X / Two;

								if ((Y - One == Zero))
								{
									StickyBit = S;
								}
							}
							else
							{
								StickyBit = S;
							}
						}
					}
				}
			}
		}
	}

	if (StickyBit == One)
	{
		message( TRUE,
			"PASSED : Sticky bit apparently used correctly.");
	}
	else
	{
		message( TRUE,
			"COMMENT: Sticky bit used incorrectly or not at all.");
	}

	TstCond (FLAW, !(GMult == NO || GDiv == NO || GAddSub == NO ||
			RMult == OTHER || RDiv == OTHER || RAddSub == OTHER),
	"Flaws are present due to lack of guard digits or round/chop failures");
	Pause();
	/*=============================================*/
	Milestone = 60;
	/*=============================================*/
	message( TRUE , "COMMENT: Testing multiplicative commutation ");
	message( FALSE, "COMMENT: with random pairs, trials = " );
	message( TRUE , int2string( NoTrials ) );

	Random9 = SQRT(3.0);
	Random1 = Third;
	I = 1;
	do
	{
		X = Random();
		Y = Random();
		Z9 = Y * X;
		Z = X * Y;
		Z9 = Z - Z9;
		I = I + 1;
	} while ( ! ((I > NoTrials) || (Z9 != Zero)));

	if (I == NoTrials)
	{
		Random1 = One + Half / Three;
		Random2 = (U2 + U1) + One;
		Z = Random1 * Random2;
		Y = Random2 * Random1;
		Z9 = (One + Half / Three) * ((U2 + U1) + One) - (One + Half /
			Three) * ((U2 + U1) + One);
	}

	if (! ((I == NoTrials) || (Z9 == Zero)))
	{
		BadCond(DEFECT, "X * Y == Y * X trial fails.\n");
	}
	else
	{
		message( TRUE,
			"PASSED : No failures found during these trials.");
	}

	Pause();
	/*=============================================*/
	Milestone = 64;
	/*=============================================*/
	message( TRUE, "COMMENT: Running test of exp(log)).");

	MinSqEr = Zero;
	MaxSqEr = Zero;

	message( FALSE, "COMMENT: random X, (exp(log(x)) - x) trials = " );
	message( TRUE , int2string( NoTrials ) );

	I = 1;
	while (I <= NoTrials)
	{
		X = Random();
		DiffExpLog();
		I = I + 1;
	}

	Pause();
	/*=============================================*/
	Milestone = 65;
	/*=============================================*/
	message( TRUE, "COMMENT: Running test of difference of two squares).");

	MinSqEr = Zero;
	MaxSqEr = Zero;
	J = Zero;
	X = Radix;
	OneUlp = U2;
	Diff2Sq (SERIOUS);
	X = BInvrse;
	OneUlp = BInvrse * U1;
	Diff2Sq (SERIOUS);
	X = U1;
	OneUlp = U1 * U1;
	Diff2Sq (SERIOUS);

	if (J != Zero)
	{
		Pause();
	}

	message( FALSE, "COMMENT: integer X, (X**2-Y**2) - (X-Y)(X+Y) trials = " );
	message( TRUE , int2string( NoTrials ) );

	J = Zero;
	X = Two;
	Y = Radix;

	if ((Radix != One)) do
	{
		X = Y;
		Y = Radix * Y;
	} while ( ! ((Y - X >= NoTrials)));

	OneUlp = X * U2;
	I = 1;
	while (I <= NoTrials)
	{
		X = X + One;
		Diff2Sq (DEFECT);
		if (J > Zero) break;
		I = I + 1;
	}

	Pause();
#ifndef 	NOTRANSCENDENTAL
	/*=============================================*/
	Milestone = 66;
	/*=============================================*/
	message( TRUE, "COMMENT: Running transcendental test 1.");

	MinSqEr = Zero;
	MaxSqEr = Zero;

	message( FALSE, "COMMENT: random X, (sin(X)**2+cos(X)**2) - 1.0 trials = " );
	message( TRUE , int2string( NoTrials ) );

	I = 1;
	while (I <= NoTrials)
	{
		X = Random();
		DiffTran1();
		I = I + 1;
	}

	Pause();
	/*=============================================*/
	Milestone = 67;
	/*=============================================*/
	message( TRUE, "COMMENT: Running transcendental test 2.");

	MinSqEr = Zero;
	MaxSqEr = Zero;

	message( FALSE, "COMMENT: random X, sin(4X) identity trials = " );
	message( TRUE , int2string( NoTrials ) );

	I = 1;
	while (I <= NoTrials)
	{
		X = Random();
		DiffTran2();
		I = I + 1;
	}

	Pause();
#endif
#ifndef 	NOHYPERBOLIC
	/*=============================================*/
	Milestone = 68;
	/*=============================================*/
	message( TRUE, "COMMENT: Running hyperbolic test 1.");

	MinSqEr = Zero;
	MaxSqEr = Zero;

	message( FALSE, "COMMENT: random X, (cosh(X)**2-sinh(X)**2) - 1.0 trials = " );
	message( TRUE , int2string( NoTrials ) );

	I = 1;
	while (I <= NoTrials)
	{
		X = Random();
		DiffHyper1();
		I = I + 1;
	}

	Pause();
#endif
	/*=============================================*/
	Milestone = 70;
	/*=============================================*/
	message( TRUE, "COMMENT: Running test of square root(x).");

	TstCond (FAILURE, (Zero == SQRT(Zero))
		   && (- Zero == SQRT(- Zero))
		   && (One == SQRT(One)), "Square root of 0.0, -0.0 or 1.0 wrong");
	MinSqEr = Zero;
	MaxSqEr = Zero;
	J = Zero;
	X = Radix;
	OneUlp = U2;
	SqXMinX (SERIOUS);
	X = BInvrse;
	OneUlp = BInvrse * U1;
	SqXMinX (SERIOUS);
	X = U1;
	OneUlp = U1 * U1;
	SqXMinX (SERIOUS);

	if (J != Zero)
	{
		Pause();
	}

	message( FALSE, "COMMENT: Testing integer X, sqrt(X * X) = X, trials = " );
	message( TRUE , int2string( NoTrials ) );

	J = Zero;
	X = Two;
	Y = Radix;

	if ((Radix != One)) do
	{
		X = Y;
		Y = Radix * Y;
	} while ( ! ((Y - X >= NoTrials)));

	OneUlp = X * U2;
	I = 1;
	while (I <= NoTrials)
	{
		X = X + One;
		SqXMinX (DEFECT);
		if (J > Zero) break;
		I = I + 1;
	}

	message( TRUE, "COMMENT: Test for sqrt monotonicity.");

	I = - 1;
	X = BMinusU2;
	Y = Radix;
	Z = Radix + Radix * U2;
	NotMonot = FALSE;
	Monot = FALSE;
	while ( ! (NotMonot || Monot))
	{
		I = I + 1;
		X = SQRT(X);
		Q = SQRT(Y);
		Z = SQRT(Z);

		if ((X > Q) || (Q > Z))
		{
			NotMonot = TRUE;
		}
		else
		{
			Q = FLOOR(Q + Half);

			if ((I > 0) || (Radix == Q * Q))
			{
				Monot = TRUE;
			}
			else if (I > 0)
			{
				if (I > 1)
				{
					Monot = TRUE;
				}
				else
				{
					Y = Y * BInvrse;
					X = Y - U1;
					Z = Y + U1;
				}
			}
			else
			{
				Y = Q;
				X = Y - U2;
				Z = Y + U2;
			}
		}
	}

	if (Monot)
	{
		message( TRUE, 
			"PASSED : sqrt has passed a test for Monotonicity.");
	}
	else
	{
		BadCond(DEFECT, "");

		message( FALSE, "COMMENT: sqrt(X) is non-monotonic for X near " );
		message( TRUE , float2string( Y ) );
	}

	Pause();
	/*=============================================*/
	Milestone = 80;
	/*=============================================*/
	MinSqEr = MinSqEr + Half;
	MaxSqEr = MaxSqEr - Half;
	Y = (SQRT(One + U2) - One) / U2;
	SqEr = (Y - One) + U2 / Eight;
	if (SqEr > MaxSqEr) MaxSqEr = SqEr;
	SqEr = Y + U2 / Eight;
	if (SqEr < MinSqEr) MinSqEr = SqEr;
	Y = ((SQRT(F9) - U2) - (One - U2)) / U1;
	SqEr = Y + U1 / Eight;
	if (SqEr > MaxSqEr) MaxSqEr = SqEr;
	SqEr = (Y + One) + U1 / Eight;
	if (SqEr < MinSqEr) MinSqEr = SqEr;
	OneUlp = U2;
	X = OneUlp;

	for( Indx = 1; Indx <= 3; ++Indx)
	{
		Y = SQRT((X + U1 + X) + F9);
		Y = ((Y - U2) - ((One - U2) + X)) / OneUlp;
		Z = ((U1 - X) + F9) * Half * X * X / OneUlp;
		SqEr = (Y + Half) + Z;

		if (SqEr < MinSqEr)
		{
			MinSqEr = SqEr;
		}

		SqEr = (Y - Half) + Z;
		if (SqEr > MaxSqEr)
		{
			MaxSqEr = SqEr;
		}

		if (((Indx == 1) || (Indx == 3))) 
		{
			X = OneUlp * Sign (X) * FLOOR(Eight / (Nine * SQRT(OneUlp)));
		}
		else
		{
			OneUlp = U1;
			X = - OneUlp;
		}
	}

	Pause();
	/*=============================================*/
	Milestone = 85;
	/*=============================================*/
	SqRWrng = FALSE;
	Anomaly = FALSE;
	RSqrt = OTHER; /* ~dgh */

	if (Radix != One)
	{
		message( TRUE, 
			"COMMENT: Testing whether sqrt is rounded or chopped.");
		D = FLOOR(Half + POW(Radix, One + Precision - FLOOR(Precision)));
	/* ... == Radix^(1 + fract) if (Precision == Integer + fract. */
		X = D / Radix;
		Y = D / A1;

		if ((X != FLOOR(X)) || (Y != FLOOR(Y)))
		{
			Anomaly = TRUE;
		}
		else
		{
			X = Zero;
			Z2 = X;
			Y = One;
			Y2 = Y;
			Z1 = Radix - One;
			FourD = Four * D;

			do
			{
				if (Y2 > Z2)
				{
					Q = Radix;
					Y1 = Y;
					do
					{
						X1 = FABS(Q + FLOOR(Half - Q / Y1) * Y1);
						Q = Y1;
						Y1 = X1;
					} while ( ! (X1 <= Zero));

					if (Q <= One)
					{
						Z2 = Y2;
						Z = Y;
					}
				}

				Y = Y + Two;
				X = X + Eight;
				Y2 = Y2 + X;
				if (Y2 >= FourD)
				{
					Y2 = Y2 - FourD;
				}
			} while ( ! (Y >= D));

			X8 = FourD - Z2;
			Q = (X8 + Z * Z) / FourD;
			X8 = X8 / Eight;

			if (Q != FLOOR(Q))
			{
				Anomaly = TRUE;
			}
			else
			{
				Break = FALSE;
				do
				{
					X = Z1 * Z;
					X = X - FLOOR(X / Radix) * Radix;
					if (X == One) 
					{
						Break = TRUE;
					}
					else
					{
						Z1 = Z1 - One;
					}
				} while ( ! (Break || (Z1 <= Zero)));

				if ((Z1 <= Zero) && (! Break))
				{
					Anomaly = TRUE;
				}
				else
				{
					if (Z1 > RadixD2)
					{
						Z1 = Z1 - Radix;
					}

					do
					{
						NewD();
					} while ( ! (U2 * D >= F9));

					if ( ((D * Radix) - D) != (W - D) )
					{
						Anomaly = TRUE;
					}
					else
					{
						Z2 = D;
						I = 0;
						Y = D + (One + Z) * Half;
						X = D + Z + Q;
						SR3750();
						Y = D + (One - Z) * Half + D;
						X = D - Z + D;
						X = X + Q + X;
						SR3750();
						NewD();
						if (D - Z2 != W - Z2)
						{
							Anomaly = TRUE;
						}
						else
						{
							Y = (D - Z2) + (Z2 + (One - Z) * Half);
							X = (D - Z2) + (Z2 - Z + Q);
							SR3750();
							Y = (One + Z) * Half;
							X = Q;
							SR3750();
							if (I == 0)
							{
								Anomaly = TRUE;
							}
						}
					}
				}
			}
		}

		if ((I == 0) || Anomaly)
		{
			BadCond(FAILURE, "Anomalous arithmetic with Integer < ");

			message( FALSE, "COMMENT: Radix^Precision = " );
			message( FALSE, float2string( W ) );
			message( TRUE , " fails test whether sqrt rounds or chops." );
			SqRWrng = TRUE;
		}

		if (! Anomaly)
		{
			if (! ((MinSqEr < Zero) || (MaxSqEr > Zero)))
			{
				RSqrt = ROUNDED;
				message( TRUE, 
				"PASSED : Square root appears to be correctly rounded." );
			}
			else
			{
				if ((MaxSqEr + U2 > U2 - Half) || (MinSqEr > Half)
					|| (MinSqEr + Radix < Half))
				{
					SqRWrng = TRUE;
				}
				else
				{
					RSqrt = CHOPPED;
					message( TRUE, 
					"PASSED : Square root appears to be chopped." );
				}
			}
		}

		if (SqRWrng)
		{
			ErrCnt [FLAW] = ErrCnt [FLAW] + 1;
			message( TRUE, 
		"FLAW   : Square root is neither chopped nor correctly rounded.");
			message( TRUE , "COMMENT: Observed errors run from " );
			message( FALSE, "COMMENT: " );
			message( FALSE, float2string( MinSqEr - Half ) );
			message( FALSE, " to " );
			message( FALSE, float2string( Half + MaxSqEr ) );
			message( TRUE, " ulps." );

			TstCond (SERIOUS, MaxSqEr - MinSqEr < Radix * Radix,
				"sqrt gets too many last digits wrong");
		}
	}
	Pause();
	/*=============================================*/
	Milestone = 90;
	/*=============================================*/
	Pause();
	message(TRUE, "COMMENT: Testing powers Z^i for small Integers Z and i.");

	N = 0;
	/* ... test powers of zero. */
	I = 0;
	Z = -Zero;
	M = 3.0;
	Break = FALSE;
	do
	{
		X = One;
		SR3980();
		if (I <= 10)
		{
			I = 1023;
			SR3980();
		}
		if (Z == MinusOne)
		{
			Break = TRUE;
		}
		else
		{
			Z = MinusOne;
			PrintIfNPositive();
			N = 0;
			/* .. if(-1)^N is invalid, replace MinusOne by One. */
			I = - 4;
		}
	} while ( ! Break);

	PrintIfNPositive();

	N1 = N;
	N = 0;
	Z = A1;
	M = FLOOR(Two * LOG(W) / LOG(A1));
	Break = FALSE;
	do
	{
		X = Z;
		I = 1;
		SR3980();
		if (Z == AInvrse)
		{
			Break = TRUE;
		}
		else
		{
			Z = AInvrse;
		}
	} while ( ! (Break));
	Pause();
	/*=============================================*/
	Milestone = 100;
	/*=============================================*/
	/*  Powers of Radix have been tested, */
	/*         next try a few primes     */
	M = NoTrials;
	Z = Three;
	do
	{
		X = Z;
		I = 1;
		SR3980();
		do
		{
			Z = Z + Two;
		} while ( Three * FLOOR(Z / Three) == Z );
	} while ( Z < Eight * Three );

	if (N > 0) {
		message( TRUE, 
		"COMMENT: Errors like this may invalidate financial calculations");
		message( TRUE, 
		"COMMENT: involving interest rates.");
	}

	PrintIfNPositive();

	N += N1;
	if (N == 0)
	{
		message( TRUE, "PASSED : ... no discrepancies found.");
	}

	Pause();
	/*=============================================*/
	Milestone = 110;
	/*=============================================*/
	message( TRUE, 
		"COMMENT: Seeking Underflow thresholds UfThold and E0.");

	D = U1;
	if (Precision != FLOOR(Precision))
	{
		D = BInvrse;
		X = Precision;
		do
		{
			D = D * BInvrse;
			X = X - One;
		} while ( X > Zero);
	}

	Y = One;
	Z = D;
	/* ... D is power of 1/Radix < 1. */
	do
	{
		C = Y;
		Y = Z;
		Z = Y * Y;
	} while ((Y > Z) && (Z + Z > Z));

	Y = C;
	Z = Y * D;

	do
	{
		C = Y;
		Y = Z;
		Z = Y * D;
	} while ((Y > Z) && (Z + Z > Z));

	HInvrse	= ( (Radix < Two) ? Two : Radix );

	H = One / HInvrse;
	/* ... 1/HInvrse == H == Min(1/Radix, 1/2) */
	CInvrse = One / C;
	E0 = C;
	Z = E0 * H;
	/* ...1/Radix^(BIG Integer) << 1 << CInvrse == 1/C */
	do
	{
		Y = E0;
		E0 = Z;
		Z = E0 * H;
	} while ((E0 > Z) && (Z + Z > Z));

	UfThold = E0;
	E1 = Zero;
	Q = Zero;
	E9 = U2;
	S = One + E9;
	D = C * S;
	if (D <= C)
	{
		E9 = Radix * U2;
		S = One + E9;
		D = C * S;
		if (D <= C)
		{
			BadCond(FAILURE, 
				"multiplication gets too many last digits wrong.\n");
			Underflow = E0;
			Y1 = Zero;
			PseudoZero = Z;
			Pause();
		}
	}
	else
	{
		Underflow = D;
		PseudoZero = Underflow * H;
		UfThold = Zero;
		do
		{
			Y1 = Underflow;
			Underflow = PseudoZero;
			if ( (E1 + E1) <= E1)
			{
				Y2 = Underflow * HInvrse;
				E1 = FABS(Y1 - Y2);
				Q = Y1;
				if ((UfThold == Zero) && (Y1 != Y2))
				{
					UfThold = Y1;
				}
			}

			PseudoZero = PseudoZero * H;
		} while ((Underflow > PseudoZero)
			&& (PseudoZero + PseudoZero > PseudoZero));
	}

	/* Comment line 4530 .. 4560 */
	if (PseudoZero != Zero)
	{
		Z = PseudoZero;
	/* ... Test PseudoZero for "phoney- zero" violates */
	/* ... PseudoZero < Underflow or PseudoZero < PseudoZero + PseudoZero
		   ... */

		if (PseudoZero <= Zero)
		{
			BadCond(FAILURE, "Positive expressions can underflow to an\n");
			message( TRUE, "COMMENT: allegedly negative value.");
			message( FALSE, "COMMENT: PseudoZero that prints out as " );
			message( TRUE , float2string( PseudoZero ) );

			X = - PseudoZero;

			if (X <= Zero)
			{
				message(TRUE, 
				"COMMENT: But -PseudoZero, which should be\n");
				message(FALSE, 
				"COMMENT: positive, isn't; it prints out as ");
				message( TRUE , float2string( X ) );
			}
		}
		else
		{
			BadCond(FLAW, "Underflow can stick at an allegedly positive\n");
			message( FALSE, "COMMENT: value PseudoZero that prints out as " );
			message( TRUE , float2string( PseudoZero ) );
		}

		TstPtUf();
	}
	Pause();
	/*=============================================*/
	Milestone = 120;
	/*=============================================*/
	if (CInvrse * Y > CInvrse * Y1)
	{
		S = H * S;
		E0 = Underflow;
	}

	if (! ((E1 == Zero) || (E1 == E0)))
	{
		BadCond(DEFECT, "");

		if (E1 < E0)
		{
			message( TRUE , "COMMENT: Products underflow at a higher" );
			message( TRUE , "COMMENT: threshold than differences." );

			if (PseudoZero == Zero) 
			{
				E0 = E1;
			}
		}
		else
		{
			message( TRUE , "COMMENT: Differences underflow at a higher" );
			message( TRUE , "COMMENT: threshold than products." );
		}
	}

	message( TRUE , "COMMENT: Smallest strictly positive number found" );
	message( FALSE, "COMMENT: is E0 = " );
	message( TRUE , float2string( E0 ) );

	Z = E0;
	TstPtUf();
	Underflow = E0;
	if (N == 1)
	{
		Underflow = Y;
	}

	I = 4;
	if (E1 == Zero)
	{
		I = 3;
	}

	if (UfThold == Zero)
	{
		I = I - 2;
	}

	UfNGrad = TRUE;
	switch (I)
	{
	case	1:
		UfThold = Underflow;
		if ((CInvrse * Q) != ((CInvrse * Y) * S))
		{
			UfThold = Y;
			BadCond(FAILURE, "Either accuracy deteriorates as numbers\n");
			message( FALSE, "COMMENT: approach a threshold = " );
			message( TRUE , float2string( UfThold ) );
			message( FALSE, " coming down from " );
			message( TRUE , float2string( C ) );
			message( TRUE ,
			"COMMENT: or else multiplication gets too many last digits wrong.");
		}
		Pause();
		break;
	
	case	2:
		BadCond(FAILURE, 
			"Underflow confuses Comparison, which alleges that\n");
		message( TRUE , "COMMENT: Q == Y while denying that |Q - Y| == 0;" );
		message( FALSE, "COMMENT: These values print out as Q = " );
		message( TRUE , float2string( Q ) );
		message( FALSE, "COMMENT: Y = " );
		message( TRUE , float2string( Y2 ) );
		message( FALSE, "COMMENT: |Q - Y| = " );
		message( TRUE , float2string( FABS(Q - Y2) ) );
		UfThold = Q;
		break;
	
	case	3:
		X = X;
		break;
	
	case	4:
		if ((Q == UfThold) && (E1 == E0)
			&& (FABS( UfThold - E1 / E9) <= E1))
		{
			UfNGrad = FALSE;
			message( FALSE, 
			"COMMENT: Underflow is gradual; Absolute roundoff error " );
			message( TRUE, "in UfThold < E0.");

			Y = E0 * CInvrse;
			Y = Y * (OneAndHalf + U2);
			X = CInvrse * (One + U2);
			Y = Y / X;
			IEEE = (Y == E0);
		}
	}

	if (UfNGrad) {
		sigsave = sigfpe;
		if (setjmp(ovfl_buf))
		{
			message( TRUE, "Underflow / UfThold failed!");
			R = H + H;
		}
		else
		{
			R = SQRT(Underflow / UfThold);
		}

		sigsave = 0;
		if (R <= H)
		{
			Z = R * UfThold;
			X = Z * (One + R * H * (One + H));
		}
		else
		{
			Z = UfThold;
			X = Z * (One + H * H * (One + H));
		}

		if (! ((X == Z) || (X - Z != Zero)))
		{
			BadCond(FLAW, "");

			message( TRUE , "COMMENT: X != Z but X - Z = Zero" );
			message( FALSE, "COMMENT: X = " );
			message( FALSE, float2string( X ) );
			message( FALSE, ", Z  = " );
			message( TRUE , float2string( Z ) );
			message( TRUE , 
				"COMMENT: This is OK only if underflow signalled." );

			sigsave = sigfpe;

			if (setjmp(ovfl_buf))
			{
				message( TRUE, "X / Z fails!");
			}
			else
			{
				message( FALSE, "COMMENT: X / Z = 1 + " );
				message( TRUE , float2string( (X / Z - Half) - Half ) );
			}

			sigsave = 0;
		}
	}

	message( FALSE, "COMMENT: The Underflow threshold is " );
	message( TRUE , float2string( UfThold ) );
	message( TRUE, 
		"COMMENT: Below this, a calculation may suffer larger Relative " );
	message( TRUE , "COMMENT: error than merely roundoff." );

	Y2 = U1 * U1;
	Y = Y2 * Y2;
	Y2 = Y * U1;
	if (Y2 <= UfThold) {
		if (Y > E0)
		{
			BadCond(DEFECT, "");
			I = 5;
		}
		else
		{
			BadCond(SERIOUS, "");
			I = 4;
		}

		message( FALSE, "COMMENT: Range is too narrow; underflow of U" );
		message( TRUE , int2string( I ) );
	}

	Pause();
	/*=============================================*/
	Milestone = 130;
	/*=============================================*/
	Y = - FLOOR(Half - TwoForty * LOG(UfThold) / LOG(HInvrse)) / TwoForty;
	Y2 = Y + Y;

	message( TRUE , "COMMENT: Since underflow occurs below the threshold");
	message( FALSE, "COMMENT: UfThold = " );
	message( FALSE, float2string( HInvrse ) );
	message( FALSE, " ^ " );
	message( TRUE , float2string( Y ) );
	message( TRUE , "COMMENT: only underflow could affect this expression.");

	V9 = POW(HInvrse, Y2);
	message( FALSE, "COMMENT: calculating yields: " );
	message( TRUE , float2string( V9 ) );

	if (! ((V9 >= Zero) && (V9 <= (Radix + Radix + E9) * UfThold)))
	{
		BadCond(SERIOUS, "this is not between 0 and underflow\n");

		message( FALSE, "COMMENT: threshold = " );
		message( TRUE , float2string( UfThold ) );
	}
	else if (! (V9 > UfThold * (One + E9)))
	{
		message( TRUE, "PASSED : This computed value is O.K.");
	}
	else
	{
		BadCond(DEFECT, "this is not between 0 and underflow\n");

		message( FALSE, "COMMENT: threshold = " );
		message( TRUE , float2string( UfThold ) );
	}

	Pause();
	/*=============================================*/
	Milestone = 140;
	/*=============================================*/
	/* ...calculate Exp2 == exp(2) == 7.389056099... */
	X = Zero;
	I = 2;
	Y = Two * Three;
	Q = Zero;
	N = 0;
	do
	{
		Z = X;
		I = I + 1;
		Y = Y / (I + I);
		R = Y + Q;
		X = Z + R;
		Q = (Z - X) + R;
	} while(X > Z);

	Z = (OneAndHalf + One / Eight) + X / (OneAndHalf * ThirtyTwo);
	X = Z * Z;
	Exp2 = X * X;
	X = F9;
	Y = X - U1;
	message( TRUE,
		"COMMENT: As X -> 1, Testing X^((X + 1) / (X - 1)) against exp(2)." );
	message( FALSE,
		"COMMENT: exp(2) = " );
	message( TRUE , float2string( Exp2 ) );

	for(I = 1;;)
	{
		Z = X - BInvrse;
		Z = (X + One) / (Z - (One - BInvrse));
		Q = POW(X, Z) - Exp2;
		if (FABS(Q) > TwoForty * U2)
		{
			N = 1;
	 		V9 = (X - BInvrse) - (One - BInvrse);
			BadCond(DEFECT, "Power function pow(x,z): ");
			message( FALSE, "COMMENT: (1 + " );
			message( FALSE, float2string( V9 ) );
			message( FALSE, ") ^ " );
			message( TRUE , float2string( Z ) );
			message( FALSE, "COMMENT: differs from correct value by " );
			message( TRUE , float2string( Q ) );
			message( TRUE, 
				"COMMENT: This may spoil financial calculations " );
			message( TRUE, 
				"COMMENT: involving tiny interest rates." );

			break;
		}
		else
		{
			Z = (Y - X) * Two + Y;
			X = Y;
			Y = Z;
			Z = One + (X - F9)*(X - F9);

			if (	(Z > One) && (I < NoTrials) )
			{
				I++;
			}
			else
			{
				if (X > One)
				{
					if (N == 0)
					{
					   message(TRUE, "PASSED : Accuracy seems adequate.");
					}

					break;
				}
				else
				{
					X = One + U2;
					Y = U2 + U2;
					Y += X;
					I = 1;
				}
			}
		}
	}
	Pause();
	/*=============================================*/
	Milestone = 150;
	/*=============================================*/
	message(TRUE, 
		"COMMENT: Testing powers Z^Q at four nearly extreme values.");

	N = 0;
	Z = A1;
	Q = FLOOR(Half - LOG(C) / LOG(A1));
	Break = FALSE;

	do
	{
		X = CInvrse;
		Y = POW(Z, Q);
		IsYeqX();
		Q = - Q;
		X = C;
		Y = POW(Z, Q);
		IsYeqX();

		if (Z < One)
		{
			Break = TRUE;
		}
		else
		{
			Z = AInvrse;
		}
	} while ( ! (Break));

	PrintIfNPositive();

	if (N == 0)
	{
		message( TRUE, "PASSED : ... no discrepancies found.");
	}

	Pause();
	/*=============================================*/
	Milestone = 160;
	/*=============================================*/
	Pause();
	message( TRUE, "COMMENT: Searching for Overflow threshold:");
	message( TRUE, "COMMENT: This may generate an error.");
	I = 0;
	Y = - CInvrse;
	V9 = HInvrse * Y;
	sigsave = sigfpe;
	if (setjmp(ovfl_buf)) { V9 = Y; goto overflow; }

	II = 0;
	do
	{
		V = Y;
		Y = V9;
		V9 = HInvrse * Y;
		++II;

		if ( II > 2000 )
		{
/*
 *			Should have overflowed by now or we have no setjmp.
 */
			break;
		}
	} while(V9 < Y);

	I = 1;
overflow:

	if ( II <= 2000 )
	{
		sigsave = 0;
		Z = V9;
		message( TRUE , "COMMENT: Can `Z = -Y' overflow?");
		message( FALSE, "COMMENT: Trying it on Y = " );
		message( TRUE , float2string( Y ) );

		V9 = - Y;
		V0 = V9;

		if (	(V - Y) == (V + V0) )
		{
			message( TRUE, "PASSED : Seems O.K.");
		}
		else
		{
			BadCond(FLAW, "-(-Y) differs from Y.\n");
		}

		if (Z != Y)
		{
			BadCond(SERIOUS, "");

			message( FALSE, "COMMENT: Overflow past " );
			message( FALSE, float2string( Y ) );
			message( FALSE, ", shrinks to " );
			message( TRUE , float2string( Z ) );
		}

		Y = V * (HInvrse * U2 - HInvrse);
		Z = Y + ((One - HInvrse) * U2) * V;

		if (Z < V0)
		{
			Y = Z;
		}

		if (Y < V0)
		{
			V = Y;
		}

		if (V0 - V < V0)
		{
			V = V0;
		}

		message( FALSE, "COMMENT: Overflow threshold is V = " );
		message( TRUE , float2string( V ) );

		if (I)
		{
			message( FALSE, "COMMENT: Overflow saturates at V0 = " );
			message( TRUE , float2string( V0 ) );
		}
		else
		{
			message( TRUE , "COMMENT: There is no saturation value." );
			message( TRUE , "COMMENT: The system traps an overflow." );
		}

		V9 = V * One;
		message( FALSE, 
			"COMMENT: No overflow should be signalled for V*1 = " );
		message( TRUE , float2string( V9 ) );

		V9 = V / One;
		message( FALSE, 
			"COMMENT: No overflow should be signalled for V/1 = " );
		message( TRUE , float2string( V9 ) );
		message( TRUE , "COMMENT: Any overflow separating V*1 from " );
		message( TRUE , "COMMENT: V above is a DEFECT." );
	}
	else
	{
		message( TRUE, "COMMENT: NO overflow signalled, test skipped." );
	}

	Pause();
	/*=============================================*/
	Milestone = 170;
	/*=============================================*/
	if (!(-V < V && -V0 < V0 && -UfThold < V && UfThold < V))
	{
		BadCond(FAILURE, "Comparisons involving ");
		message( FALSE, "COMMENT: +-" );
		message( FALSE, float2string( V ) );
		message( FALSE, ", +-" );
		message( FALSE, float2string( V0 ) );
		message( FALSE, ",  and +-" );
		message( TRUE , float2string( UfThold ) );
		message( TRUE , "COMMENT: are confused by Overflow." );
	}

	Pause();
	/*=============================================*/
	Milestone = 175;
	/*=============================================*/
	for(Indx = 1; Indx <= 3; ++Indx)
	{
		switch (Indx)
		{
			case 1: Z = UfThold; break;
			case 2: Z = E0; break;
			case 3: Z = PseudoZero; break;
		}

		if (Z != Zero)
		{
			V9 = SQRT(Z);
			Y = V9 * V9;
			if (Y / (One - Radix * E9) < Z
			   || Y > (One + Radix * E9) * Z)
			{ /* dgh: + E9 --> * E9 */
				if (V9 > U1)
				{
					BadCond(SERIOUS, "");
				}
				else
				{
					BadCond(DEFECT, "");
				}

				message( FALSE, 
					"COMMENT: Comparison alleges that what prints as Z =");
				message( TRUE, float2string( Z ) );
				message( FALSE, 
					"COMMENT: is too far from ");
				message( TRUE, float2string( Y ) );
			}
		}
	}

	Pause();
	/*=============================================*/
	Milestone = 180;
	/*=============================================*/
	for(Indx = 1; Indx <= 2; ++Indx)
	{
		if (Indx == 1)
		{
			Z = V;
		}
		else
		{
			Z = V0;
		}

		V9 = SQRT(Z);
		X = (One - Radix * E9) * V9;
		V9 = V9 * X;
		if (((V9 < (One - Two * Radix * E9) * Z) || (V9 > Z)))
		{
			Y = V9;
			if (X < W)
			{
				BadCond(SERIOUS, "");
			}
			else
			{
				BadCond(DEFECT, "");
			}

			message( FALSE, 
				"COMMENT: Comparison alleges that Z = ");
			message( TRUE, float2string( Z ) );
			message( FALSE, 
				"COMMENT: is too far from sqrt(Z) ^ 2 =  ");
			message( TRUE, float2string( Y ) );
		}
	}

	Pause();
	/*=============================================*/
	Milestone = 190;
	/*=============================================*/
	Pause();
	X = UfThold * V;
	Y = Radix * Radix;

	if (X*Y < One || X > Y)
	{
		if (	((X * Y) < U1) || (X > (Y/U1)) )
		{
			BadCond(DEFECT, "Badly");
		}
		else
		{
			BadCond(FLAW, "");
		}

		message( FALSE, "COMMENT: unbalanced range: UfThold * V = " );
		message( TRUE , float2string( X ) );
		message( TRUE , "is too far from 1." );
	}

	Pause();
	/*=============================================*/
	Milestone = 200;
	/*=============================================*/
	for (Indx = 1; Indx <= 5; ++Indx)
	{
		X = F9;
		switch (Indx)
		{
			case 2: X = One + U2; break;
			case 3: X = V; break;
			case 4: X = UfThold; break;
			case 5: X = Radix;
		}

		Y = X;
		sigsave = sigfpe;

		if (setjmp(ovfl_buf))
		{
			message( FALSE, "COMMENT: X / X  traps when X = " );
			message( TRUE , float2string( X ) );
		}
		else
		{
			V9 = (Y / X - Half) - Half;
			if (V9 == Zero)
			{
				continue;
/*				=========				*/
			}

			if (V9 == - U1 && Indx < 5)
			{
				BadCond(FLAW, "");
			}
			else
			{
				BadCond(SERIOUS, "");
			}

			message( FALSE, "COMMENT: X / X  differs from 1 when X =" );
			message( TRUE , float2string( X ) );
			message( FALSE, "COMMENT: X / X  - 1/2 - 1/2 =" );
			message( TRUE , float2string( V9 ) );
		}

		sigsave = 0;
	}

	Pause();
	/*=============================================*/
	Milestone = 210;
	/*=============================================*/
#ifndef	NOZERODIVIDE
	MyZero = Zero;
	sigsave = sigfpe;
	message( FALSE, "COMMENT: Trying to compute 1/0 gives ");
	if (!setjmp(ovfl_buf))
	{
		message( TRUE , float2string( One / MyZero ) );
	}

	sigsave = sigfpe;
	message( FALSE, "COMMENT: Trying to compute 0/0 gives ");
	if (!setjmp(ovfl_buf))
	{
		message( TRUE , float2string( Zero / MyZero ) );
	}

	sigsave = 0;
	Pause();
#endif
	/*=============================================*/
	Milestone = 220;
	/*=============================================*/

	message( TRUE , "COMMENT: =========================================" );
	message( TRUE , "COMMENT:     Embedded System Paranoia SUMMARY" );
#ifdef	SINGLE
	message( FALSE, "COMMENT:         SINGLE PRECISION " );
#else
#ifdef	LONG_DOUBLE
	message( FALSE, "COMMENT:       LONG DOUBLE PRECISION " );
#else
	message( FALSE, "COMMENT:         DOUBLE PRECISION " );
#endif
#endif
	message( FALSE, int2string( sizeof(FLOAT) * 8) );
	message( TRUE , " bits" );
	message( FALSE , "COMMENT: Closest separation = " );
	message( TRUE  , float2string( U1 ) );
	message( TRUE , "COMMENT:" );

	{
		static char *msg[] = {
			"FAILUREs encountered       = ",
			"SERIOUS DEFECTs discovered = ",
			"DEFECTs discovered         = ",
			"FLAWs discovered           = " };
		int i;

		for(i = 0; i < 4; i++)
		{
			message( FALSE, "COMMENT: Number of " );
			message( FALSE, msg[i] );
			message( TRUE , int2string( ErrCnt[i] ) );
		}
	}

	message( TRUE , "COMMENT:" );

	if ((ErrCnt[FAILURE] + ErrCnt[SERIOUS] + ErrCnt[DEFECT]
			+ ErrCnt[FLAW]) > 0) {
		if ((ErrCnt[FAILURE] + ErrCnt[SERIOUS] + ErrCnt[
			DEFECT] == 0) && (ErrCnt[FLAW] > 0))
		{

			message( TRUE , 
				"PASSED : The arithmetic diagnosed seems satisfactory " );
			message( TRUE , "COMMENT: though flawed." );

			plevel	= 3;
		}

		if ((ErrCnt[FAILURE] + ErrCnt[SERIOUS] == 0)
			&& ( ErrCnt[DEFECT] > 0))
		{
			message( TRUE , 
				"COMMENT: The arithmetic diagnosed may be Acceptable " );
			message( TRUE , "COMMENT: despite inconvenient DEFECT." );
			plevel	= 4;
		}

		if ((ErrCnt[FAILURE] + ErrCnt[SERIOUS]) > 0)
		{
			message( TRUE , 
				"FAILED : The arithmetic diagnosed has unacceptable" );
			message( TRUE , "COMMENT: Serious DEFECT." );
			plevel	= 5;
		}

		if (ErrCnt[FAILURE] > 0)
		{
			message( TRUE , 
				"FAILED : Potentially fatal FAILURE has occurred" );
			plevel	= 6;
		}
	}
	else
	{
		message( TRUE , 
			"PASSED : No failures, defects nor flaws have been discovered.");

		if (! ((RMult == ROUNDED) && (RDiv == ROUNDED)
			&& (RAddSub == ROUNDED) && (RSqrt == ROUNDED))) 
		{
			message( TRUE , 
				"PASSED : The arithmetic diagnosed seems satisfactory.");
			plevel	= 2;
		}
		else
		{
			if (StickyBit >= One &&
				(Radix - Two) * (Radix - Nine - One) == Zero)
			{
				message( FALSE , 
				"COMMENT: Rounding appears to conform to IEEE standard P");

				if ((Radix == Two) &&
					 ((Precision - Four * Three * Two) *
					  ( Precision - TwentySeven -
					   TwentySeven + One) == Zero)) 
				{
					message( TRUE, "754");
				}
				else
				{
					message( TRUE, "854");
				}

				if (IEEE)
				{
					message( TRUE, ".");
				}
				else
				{
					message( TRUE ,
						"COMMENT: , except for possibly Double Rounding");
					message( TRUE ,
						"COMMENT: during Gradual Underflow.");
				}
			}

			message( TRUE , 
				"PASSED : The arithmetic diagnosed seems Excellent.");
			plevel	= 1;
		}
	}

	message( TRUE , "COMMENT:" );
	message( TRUE , "COMMENT: Rating ...");
	message( TRUE , "COMMENT:" );

	finalScore( (plevel == 1), "Excellent" );
	finalScore( (plevel == 2), "Very good" );
	finalScore( (plevel == 3), "Good" );
	finalScore( (plevel == 4), "Acceptable" );
	finalScore( (plevel == 5), "Unacceptable" );
	finalScore( (plevel == 6), "Broken" );

	if (fpecount)
	{
		message( FALSE , 
			"COMMENT: Total floating point exceptions registered = ");
		message( TRUE , int2string( fpecount ) );
	}

	otherScores();

	message( TRUE , "COMMENT:" );
	message( TRUE,  "COMMENT: END OF TEST.");
	message( TRUE , "COMMENT: =========================================" );

	exit(EXIT_SUCCESS);
}
/*---------------------------------------------------------------*/
