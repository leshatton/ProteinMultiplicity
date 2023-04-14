#!/bin/bash
#
#	Makes plot of multiplicity v. length.
#-------------------------------------------------------
if test $# -ne 2
then
	echo "Usage: $0 [version, e.g. 18-02] [trembl|swissprot]"
	exit
fi

VERSION=$1
DB=$2

if test "$DB" = "trembl"
then
	REPFILE=${VERSION}_${DB}_protein_sharing.csv
elif test "$DB" = "swissprot"
then
	REPFILE=${VERSION}_${DB}_protein_sharing.csv
else
	echo "Unknown database source, $DB"
	exit
fi

if test ! -f $REPFILE
then
	echo "$REPFILE does not exist, create with 2-prepareData.sh"
	exit
else
#
#	Note that we use a .tx2 suffix so that it is not saved in RESULTS later as it may
#	be large.
#
	if test ! -f multiplicity_v_lengths_scatter.tx2
	then
		cat $REPFILE | awk 'BEGIN{FS=","}{print $1,$5; }' \
			> multiplicity_v_lengths_scatter.tx2
	fi
fi
#
#	We create this as .png file as the .eps file is enormous.
#
gnuplot	multiplicity_v_lengths_scatter.gnu
convert	-density 300 multiplicity_v_lengths_scatter.eps Fig4.png
rm multiplicity_v_lengths_scatter.eps
