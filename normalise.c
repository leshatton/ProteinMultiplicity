/*
 *	Program to normalise (# occurrences/value) pairs output by the
 *	extract_lines_xx.sh shells from real programs in C, Fortran and TCL.
 *
 *	The # occurrences and the value are independently normalised to unity.
 *	This program is used from the various scripts to normalise
 *	the data and then uses gnuplot to generate .eps files of the results.
 *
 *	Author Les Hatton.  This software is distributed under the conditions
 *	of the GNU public licence.
 *
 *	Revision:	$Revision: 1.1 $
 *	Date:	$Date: 2019/12/12 14:19:23 $
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	ABS(A,B)	(((A)-(B) > 0.0) ? ((A)-(B)) : ((B)-(A)) )

enum {byrank, bysize, byccdf, byccdfn};

int
main( int argc, char *argv[])
{
	FILE		*fp = NULL;
	double	number;
	double	value;
	int		rank;
	int		mode = byrank;
	double	accum = 0.0;
	double	total = 0.0;
	double	mxvalue = 0.0;

	if ( (argc >= 2) 
		&& (strcmp( argv[1], "rank" ) == 0) )
	{
/*
 *		Rank-ordered.
 */
		mode	= byrank;
	}
	else if ( (argc >= 2) 
		&& (strcmp( argv[1], "ccdf" ) == 0) )
	{
/*
 *		CCDF a la Newman.
 */
		mode	= byccdf;
	}
	else if ( (argc >= 2) 
		&& (strcmp( argv[1], "ccdfn" ) == 0) )
	{
/*
 *		CCDF a la Newman but with y axis normalised.
 */
		mode	= byccdfn;
	}
	else
	{
/*
 *		Size ordered.
 */
		mode	= bysize;
	}

	if ( (fp = tmpfile()) == NULL )
	{
		fprintf(stderr,"Couldn't open tmpfile()\n");
		exit(EXIT_FAILURE);
	}
/*
 *	Read through the list of # occurrences / value pairs.
 */
	while ( scanf("%lf %lf", &number, &value ) != EOF )
	{
		total	+= number;
		if ( value > mxvalue )	mxvalue	= value;

		fprintf(fp, "%lf %lf\n",number,value);
	}

	rewind(fp);

	printf("# x,\n");
/*
 *	Output the pairs.
 */
	rank		= 0;
	while ( fscanf(fp, "%lf %lf", &number, &value ) != EOF )
	{
		accum	+= number;
		++rank;

		if ( mode == bysize )
		{
			printf("%lf %lf\n", value, (number/total));
		}
		else if ( mode == byccdf )
		{
			printf("%lf %lf\n", value, (total - accum));
		}
		else if ( mode == byccdfn )
		{
			printf("%lf %lf\n", value, (total - accum)/total);
		}
		else
		{
			printf("%lf %lf\n", (double) rank, (number/total));
		}
	}

	printf("\n\n");

	exit(EXIT_SUCCESS);
}
