/*----------------------------------------------------------------
 *	Fast protein sequence comparison.
 *
 *	Copyright 2018, Les Hatton, lesh@oakcomp.co.uk, KT3 3AJ, U.K.
 *
 *	Released under GPL.
 *
 ----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 *	String closeness.
 */
#define	STRINGS_SAME			0
#define	STRINGS_HAVE_TRANSPOSE	1
#define	STRINGS_HAVE_ONE_DIFF	2
#define	STRINGS_HAVE_MULT_DIFF	3
#define	STRINGS_HAVE_OZERO_DIFF	4
#define	STRINGS_HAVE_LONE_DIFF	5
#define	STRINGS_HAVE_2ZED_DIFF	6
#define	STRINGS_HAVE_5ESS_DIFF	7
#define	STRINGS_HAVE_SNSH_DIFF	8

#define	LARGE_BUFFER			1024000
static	char	buf[LARGE_BUFFER];
static	char	seq[LARGE_BUFFER];
static	char	last_seq[LARGE_BUFFER];

static	long	len			= 0;
static	long	last_len		= 0;
static	const int	NEAR		= 2;

static	long	total		= 0;
static	long	ntrans		= 0;
static	long	nonediff		= 0;
/*---------------------------------------------------------------*/
#define	EQ(s,t)	(strcmp(s,t) == 0)

/*
 *   Compute the "distance" between two names using the following
 *	metric:
 *
 *		Return STRINGS_SAME if the strings are identical.
 *		Return STRINGS_HAVE_TRANSPOSE if the strings differ by one 
 *			character transposed.
 *		Return STRINGS_HAVE_ONE_DIFF if the strings differ by one 
 *			character, wrong, added or deleted.
 *		Return STRINGS_HAVE_OZERO_DIFF if the strings differ by 
 *			an O - zero replacement.
 *		Return STRINGS_HAVE_LONE_DIFF if the strings differ by 
 *			an l - one replacement.
 *		Return STRINGS_HAVE_MULT_DIFF if the strings differ by some 
 *			other amount.
 */
int
myc_spell_dist( char * s, char * t )
{
	while (*s++ == *t )
	{
		if ( *t++ == '\0' )
			/*	identical		*/
			return (STRINGS_SAME);
	}
/*
 *	Here if not identical.
 */
	if ( *--s )
	{
		if ( *t )
		{
			if ( s[1] && t[1] && 
				*s == t[1] && *t == s[1] &&
				EQ(s+2,t+2) )
			{
				/*	transposition		*/
				return(STRINGS_HAVE_TRANSPOSE);
			}
			if ( EQ(s+1,t+1) )
			{
				/*	1 character mismatch*/
				if ( (*s == '0' && *t == 'O') ||
					(*s == 'O' && *t == '0') 	)
				{
/*
 *					O - zero difference.
 */
					return(STRINGS_HAVE_OZERO_DIFF);
				}
				else if ( (*s == 'l' && *t == '1') ||
						(*s == '1' && *t == 'l') 	)
				{
/*
 *					ell - 1 difference.
 */
					return(STRINGS_HAVE_LONE_DIFF);
				}
				else if ( (*s == '2' && *t == 'Z') ||
						(*s == 'Z' && *t == '2') 	)
				{
/*
 *					2 - Z difference.
 */
					return(STRINGS_HAVE_2ZED_DIFF);
				}
				else if ( (*s == '5' && *t == 'S') ||
						(*s == 'S' && *t == '5') 	)
				{
/*
 *					5 - S difference.
 */
					return(STRINGS_HAVE_5ESS_DIFF);
				}
				else if ( (*s == 'n' && *t == 'h') ||
						(*s == 'h' && *t == 'n') 	)
				{
/*
 *					n - h difference.
 */
					return(STRINGS_HAVE_SNSH_DIFF);
				}
				else
				{
					return(STRINGS_HAVE_ONE_DIFF);
				}
			}
		}
		if ( EQ(s+1,t) )
		{
			/*	extra character	*/
			return(STRINGS_HAVE_ONE_DIFF);
		}
	}
	if ( *t && EQ(s,t+1) )
	{
		/*	missing character	*/
		return(STRINGS_HAVE_ONE_DIFF);
	}
	else
	{
		/*	other differences	*/
		return(STRINGS_HAVE_MULT_DIFF);
	}
}
/*---------------------------------------------------------------*/
/*
 *	The strings arrive on the standard input.
 */
int main(void)
{
	long	linecount	= 0;

	while( fgets( buf, LARGE_BUFFER, stdin ) != NULL )
	{
		++linecount;

		long				len	= 0;
		volatile 	long		i	= 0;
		volatile	long		j	= 0;
		volatile	long		k	= 0;
		char				clen[BUFSIZ];
/*
 *		Get the length.
 */
		for ( i = 0; buf[i] != ','; ++i )
		{
			clen[i]	= buf[i];
		}
	
		clen[i]	= '\0';
	
		len		= atoi( clen );
/*
 *		Get the sequence.
 */
		for ( j = i+1; buf[j] != ','; ++j )
		{
/*
 *			SKIP
 */
		}

		for ( k = j+1; buf[k] != ','; ++k )
		{
			if ( k-(j+1) >= LARGE_BUFFER )
			{
				fprintf(stderr,"Buffer exceeded.  k-(j+1) = %ld\n",k-(j+1));
				exit(EXIT_FAILURE);
			}

			seq[k-(j+1)]	= buf[k];
		}

		seq[k-(j+1)]	= '\0';
/*
		printf("len = %ld, seq = %s\n", len, seq);
 */

		if ( len < last_len )
		{
			fprintf(stderr,"Input csv file not in ascending sort order\n");
			exit(EXIT_FAILURE);
		}
/*
 *		OK, we have the length and sequence for the current line.
 */
		if ( last_len == 0 )
		{
			last_len	= len;
			strncpy( last_seq, seq, LARGE_BUFFER );

			continue;
/*			=========		*/
		}

		++total;
/*
 *		Here if not first sequence.
 */
		if ( (len - last_len) < NEAR )
		{
/*
 *			Close enough to compare
 */
			int	val	= myc_spell_dist( seq, last_seq );

			switch( val )
			{
			case STRINGS_HAVE_TRANSPOSE:
				++ntrans;
				printf("Transpose on line %ld\n",linecount);
				fflush(stdout);
				break;

			case STRINGS_HAVE_ONE_DIFF:
				printf("One-diff  on line %ld\n",linecount);
				fflush(stdout);
				++nonediff;
				break;

			default:
				break;
			}
		}
		else
		{
/*
 *			Not close enough, ignore.
 */
		}

		last_len	= len;
		strncpy( last_seq, seq, LARGE_BUFFER );
	}

	printf("Total                          = %ld\n", total);
	printf("Number transpose possibilities = %ld\n", ntrans);
	printf("Number one-diff possibilities  = %ld\n", nonediff);

	exit(EXIT_SUCCESS);
}
