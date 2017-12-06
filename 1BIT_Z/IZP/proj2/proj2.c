/**
 *	Second project IZP - Iterative computations
 *	Filip Kocica
 *	xkocic01
 *	version 1.0
 *	Creation date:	05.11.2016
 *	Last change:	18.11.2016
*/

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define EPS					1e-8
#define CHAR_ZERO			'0'
#define CHAR_NINE			'9'	

enum
{
	R_LOG = 1,
	R_POW = 2,
	R_MY_LOG = 3,
	R_MY_POW = 4
} input;

enum
{
	ERR_UNDEF_IN = -1,
	ERR_ITERATIONS = -2,
	ERR_NAN = -3
} error;

bool isNum(char *s);
bool validNumber(double x, double y);
int inParam(int argc, char *argv[]);

double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);

double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);

void runLog(double x, unsigned int n);
void runPow(double x, double y, unsigned int n);
void runMyLog(double x);
void runMyPow(double x, double y);

void phelp();
void myperror(int myerrno);

double mylog(double x);
double mypow(double x, double y);


int main(int argc, char **argv)
{
	switch(inParam(argc, argv))
	{
		case R_LOG:
			runLog(atof(argv[2]), atoi(argv[3]));
			break;

		case R_POW:
			runPow(atof(argv[2]), atof(argv[3]), atoi(argv[4]));
			break;

		case R_MY_LOG:
			runMyLog(atof(argv[2]));
			break;

		case R_MY_POW:
			runMyPow(atof(argv[2]),atof(argv[3]));
			break;

		case ERR_UNDEF_IN:
			phelp();
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
 *	@param argc - number of arguments passed to program
 *	@param argv - pointer to array of arrays with args passed to program
 *	@return enum value
 *
 *	function finds out which function should be run, based on
 *	intput arguments. if none is found, function returns ERROR.
*/
int inParam(int argc, char **argv)
{
	switch(argc)
	{
		case 3:
			if ( !isNum(argv[2]) )
			{
				myperror(ERR_NAN);
			}
			else if ( strcmp("--mylog", argv[1]) == 0 )
			{
				return R_MY_LOG;
			}
			break;

		case 4:
			if ( !(isNum(argv[2]) && isNum(argv[3])) )
			{
				myperror(ERR_NAN);
			}
			else if ( strcmp("--log", argv[1]) == 0 )
			{
				if ( atoi(argv[3]) <= 0 )
				{
					myperror(ERR_ITERATIONS);
					break;
				}
				return R_LOG;
			}
			else if ( strcmp("--mypow", argv[1]) == 0 )
			{
				return R_MY_POW;
			}
			break;

		case 5:
			if ( atoi(argv[4]) <= 0 )
			{
				myperror(ERR_ITERATIONS);
			}
			else if ( !(isNum(argv[2]) && isNum(argv[3]) && isNum(argv[4])) )
			{
				myperror(ERR_NAN);
			}
			else if ( strcmp("--pow", argv[1]) == 0 )
			{
				return R_POW;
			}
			break;
	}
	return ERR_UNDEF_IN;
}

/**
 *	@param x - number passed to program we want to calculate
 *	@param n - number of iterations, shows the expansion of the polynom
 *	@return calculated value
 *
 *	function calculates logarithm using taylor polynom on n iterations.
*/
double taylor_log(double x, unsigned int n)
{
	double act = 0.0;
	double result = 0.0;

	if (x > 1.0)
	{
		act = (x-1) / x;
		result = act;

		for(unsigned int i = 2; i <= n; i++)
		{
			act = act * ((x-1) / x);
			result += act / i;
		}

		return result;
	}

	x = 1.0 - x;
	act = x;
	result -= act;

	for(unsigned int i = 2; i <= n; i++)
	{
		act = act * x;
		result -= act / i;
	}

	return result;
}

/**
 *	@param x - number passed to program we want to calculate
 *	@param n - number of iterations, shows the expansion of the polynom
 *	@return calculated value
 *
 *	function calculates logarithm using continued fractions on n iterations.
*/
double cfrac_log(double x, unsigned int n)
{
	x = -1*(1-x)/(1+x);
	double result = 0.0;
	double calculatedPowX = x*x;

	for(; n > 1; n--)
	{
		result = (((n-1)*(n-1)) * calculatedPowX) / ((2*n-1) - result);
	}

	result = (2.0*x) / (1-result);

	return result;
}

/**
 *	@param x,y - numbers passed to program we want to calculate
 *	@param n - number of iterations, shows the expansion of the polynom
 *	@return calculated value
 *
 *	function calculates Taylor polynom for exponential function
 *	using taylor_log on n iterations.
*/
double taylor_pow(double x, double y, unsigned int n)
{
	double eta = 1.0;
	double act = 1.0;

	for(unsigned int i = 1; i <= n; i++)
	{
		act *= ((y * taylor_log(x, n)) /  i);
		eta += act;
	}

	return eta;
}

/**
 *	@param x,y - numbers passed to program we want to calculate
 *	@param n - number of iterations, shows the expansion of the polynom
 *	@return calculated value
 *
 *	function calculates Taylor polynom for exponential function
 *	using cfrac_log on n iterations.
*/
double taylorcf_pow(double x, double y, unsigned int n)
{
	double eta = 1.0;
	double act = 1.0;

	for(unsigned int i = 1; i <= n; i++)
	{
		act *= ((y * cfrac_log(x, n)) /  i);
		eta += act;
	}

	return eta;
}

/**
 *	@param x - number passed to program we want to calculate
 *	@param n - number of iterations, shows the expansion of the polynom
 *	@return nothing
 *
 *	function prints calculated values of log and implemented polynoms. 
*/
void runLog(double x, unsigned int n)
{
	double cfrac;
	double taylor;

	if (!x)
	{
		cfrac = taylor = -INFINITY;
	} 
	else if (x < 0.0)
	{
		cfrac = taylor = NAN;
	}
	else
	{
		cfrac = cfrac_log(x,n);
		taylor = taylor_log(x,n);
	}

	printf("       log(%g) = %.12g\n", x, log(x));
	printf("    cf_log(%g) = %.12g\n", x, cfrac);
	printf("taylor_log(%g) = %.12g\n", x, taylor);
}

/**
 *	@param x,y - numbers passed to program we want to calculate
 *	@param n - number of iterations, shows the expansion of the polynom
 *	@return nothing
 *
 *	function prints calculated values of pow and implemented polynoms. 
*/
void runPow(double x, double y, unsigned int n)
{
	double taylorcf;
	double taylor;

	if (x <= 0.0)
	{
		taylorcf = taylor = NAN;
	}
	else
	{
		taylorcf = taylorcf_pow(x,y,n);
		taylor = taylor_pow(x,y,n);
	}

	printf("         pow(%g,%g) = %.12g\n", x, y, pow(x,y));
	printf("  taylor_pow(%g,%g) = %.12g\n", x, y, taylor);
	printf("taylorcf_pow(%g,%g) = %.12g\n", x, y, taylorcf);
}

/**
 *	@param x - number passed to program we want to calculate
 *	@return calculated value
 *
 *	function calculates value accurate on 8 significant digits
 *	with function log from math.h on the lowest iterations,
 *	that is possible.
*/
double mylog(double x)
{
	double taylor = 0.0, ptaylor;
	double cfrac = 0.0, pcfrac;
	int iter = 0;

	do
	{
		iter++;

		ptaylor = taylor;
		pcfrac = cfrac;
		
		taylor = taylor_log(x, iter);
		cfrac  = cfrac_log(x, iter);

		if (validNumber(cfrac - pcfrac, taylor - ptaylor))
		{
			return NAN;
		}
	}
	while (fabs(taylor - ptaylor) > EPS && fabs(cfrac - pcfrac) > EPS);

	//printf("Minimalni pocet iteraci: %d\n\n", iter);
	
	if (fabs(taylor - ptaylor) <= EPS)
		return taylor;
	else
		return cfrac;
}

/**
 *	@param x, y - numbers passed to program we want to calculate
 *	@return calculated value
 *
 *	function calculates value accurate on 8 significant digits
 *	with function pow from math.h on the lowest iterations,
 *	that is possible.
*/
double mypow(double x, double y)
{
	double taylor = 0.0, ptaylor;
	double cfrac = 0.0, pcfrac;
	int iter = 0;

	do
	{
		iter++;

		ptaylor = taylor;
		pcfrac = cfrac;
		
		taylor = taylor_pow(x, y, iter);
		cfrac  = taylorcf_pow(x, y, iter);

		if (validNumber(cfrac - pcfrac, taylor - ptaylor))
		{
			return NAN;
		}
	}
	while (fabs(taylor - ptaylor) > EPS && fabs(cfrac - pcfrac) > EPS);

	//printf("Minimalni pocet iteraci: %d\n\n", iter);
	
	if (fabs(taylor - ptaylor) <= EPS)
		return taylor;
	else
		return cfrac;
}

/**
 *	@param x, y - numbers we want to test
 *	@return boolean value representing that numbers are valid or not
 *
 *	returns false if none of x,y is NAN or INF, otherwise true 
*/
bool validNumber(double x, double y)
{
	return x == NAN || x == INFINITY || y == NAN || y == INFINITY;
}

/**
 *	@param s - pointer to array of ch we want to test on number
 *	@return 1 if only numbers, or 0
 *
 *	Checks out if all chars in string are numbers or '.' and skips minus
*/
bool isNum(char *s)
{
	int i=0, len = (int)strlen(s);					// find out len of str

	if(s[0] == '-' || s[0] == '+')					// skip minus
		i++;

	for(; i < len; ++i)
	{
		if ( ( CHAR_ZERO > s[i] || s[i] > CHAR_NINE ) && s[i] != '.' )
		{
			return false;
		}	
	}

	return true;
}

/**
 *	@param myerrno - code of error
 *	@return function returns nothing
 *
 *	Prints one of strings on stdout, based on code of error
*/
void myperror(int myerrno)
{
	switch (myerrno)
	{
		case ERR_ITERATIONS:
			printf("\nERROR: Unexpected input. Number of iterations "
				   "has to be greater than 0.\n");
		break;

		case ERR_NAN:
			printf("\nERROR: Unexpected input. Enter the number.\n");
		break;

	}
}

/**
 *	@param function takes no params
 *	@return function returns nothing
 *
 *	if case of wrong input function prints help
*/
void phelp()
{
	fprintf(stderr,
	"\nProgram runs in the following form:\n\n"
   	"./proj2 --log X N\t\tN > 0\n"
   	"./proj2 --pow X Y N\t\tN > 0\n"
   	"./proj2 --mylog X\n"
   	"./proj2 --mypow X Y\n"
	"\n--log using Taylor polynom and Continued fraction"
	" calculates\nlogarithm of X, with accuracy on N iterations."
	"\n--pow using Exponential function and logarithms calculates"
	" x^y\n with accuracy on N iterations."
	"\n\n*** Premium functions ***"
	"\n--mylog elect more accurate calculation of logarithm"
	" on 8 significant \ndigits corresponding to output .7e"
	"on the lowest iterations"
	"\n--mypow is doing same as mylog, although with calculation"
	" of pow.\n\n");
}

/**
 *	@param x - number passed to program
 *	@return function returns nothing
 *
 *	Prints referential log of math.h and calculated premium func mylog
*/
void runMyLog(double x)
{
	double varmylog;

	if(!x)
	{
		varmylog = -INFINITY;
	}
	else if(x < 0.0)
	{
		varmylog = NAN;
	}
	else
	{
		varmylog = mylog(x);
	}

	printf("  log(%g) = %.7e\n", x, log(x));
	printf("mylog(%g) = %.7e\n", x, varmylog);
}

/**
 *	@param x, y - numbers passed to program 
 *	@return function returns nothing
 *
 *	Prints referential pow of math.h and calculated premium func mypow
*/
void runMyPow(double x, double y)
{
	double varmypow;

	if(x <= 0.0)
	{
		varmypow = NAN;
	}
	else
	{
		varmypow = mypow(x, y);
	}

	printf("  pow(%g,%g) = %.7e\n", x, y, pow(x,y));
	printf("mypow(%g,%g) = %.7e\n", x, y, varmypow);
}





