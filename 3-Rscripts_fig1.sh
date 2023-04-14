#!/bin/sh
#
#	We analyse the data with R.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#-------------------------------------------------
if test $# -ne 5
then
	echo "Usage: $0 [version, e.g. 18-02] [trembl|sprot] [Rank low] [Rank high] [Num. sims]"
	exit
fi

VERSION=$1
DB=$2
X0=$3
X1=$4
SIMS=$5

if test "$DB" = "trembl"
then
	REPLICAS=${VERSION}_${DB}_protein_replicas_ccdf.dat
	RANALYSIS=${VERSION}_${DB}_Ranalyses_fig1.out
elif test "$DB" = "swissprot"
then
	REPLICAS=${VERSION}_${DB}_protein_replicas_ccdf.dat
	RANALYSIS=${VERSION}_${DB}_Ranalyses_fig1.out
else
	echo "Unknown database source, $DB"
	exit
fi

if test ! -f $REPLICAS
then
	echo "File $REPLICAS is missing.  You need to run 3-fig1.sh first."
	exit.
fi

rm -f $RANALYSIS
#
#	Do the R analysis.
#
echo "R analysis for slope of combined ccdf for $REPLICAS"	>> $RANALYSIS
cat $REPLICAS \
	| grep -v "^#" \
	| awk -v x0=$X0 -v x1=$X1 '{if ( $2 > 0.0 && $1 > x0 && $1 <= x1 ) print log($1);}'    \
	> uni_x.txt
cat $REPLICAS \
	| grep -v "^#" \
	| awk -v x0=$X0 -v x1=$X1 '{if ( $2 > 0.0 && $1 > x0 && $1 <= x1 ) print log($2);}'    \
	> uni_y.txt

Rscript	plot_tail_proteinreuse.R						>> $RANALYSIS
echo "================================================="	>> $RANALYSIS

echo "R analysis for Clauset sufficiency test for $REPLICAS"	>> $RANALYSIS
#-------------------------------------------------
#	Now the Clauset test for sufficiency.
#
cat	$REPLICAS \
	| grep -v "^#" \
	| grep -v "^$" \
	| awk -v x0=$X0 -v x1=$X1 '{if ( $2 > 0.0 && $1 > x0 && $1 <= x1 ) print $1,$2;}'    \
	> length_powerlawtest.txt
#
#	Specify number of simulations to use.
#
NSMS=$SIMS
echo `date`									>> $RANALYSIS
echo "DAT $REPLICAS"							>> $RANALYSIS
echo "FITRANGE $X0 - $X1"						>> $RANALYSIS
Rscript	lengths_analysis.R	$X0 $X1				>> $RANALYSIS
Rscript	powerlaw_test.R	$X0 $X1 $NSMS			>> $RANALYSIS
#-------------------------------------------------

echo "The R analyses are in $RANALYSIS"
