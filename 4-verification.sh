#!/bin/sh
#
#	Performs final verification checking on each .dat file used
#	in the various figures and tables.
#
#	Copyright Les Hatton, 2013-.
#	Released under the GNU GPL.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#----------------------------------------------------------------
rm	-f temp$$.dat
LOGFILE=reproducibility.log

changed=no
missing=no
#
#	We start by collecting all the results from the main directory.
#	We only regress on the .dat files since the .txt and .eps files
#	are derived from them.
#
mkdir -p RESULTS
mv *.dat *.txt	*.eps	RESULTS/

echo "**********************************************"	>> $LOGFILE
echo "Regression suite run on `date`"				>> $LOGFILE

for file in REGRESSION/*.dat
do
	bname=`basename $file`
	if test ! -f RESULTS/$bname
	then
		missing=yes
		echo "File RESULTS/$bname not yet generated: unable to verify"
	else
		diff -yw RESULTS/$bname $file	> temp$$.dat
		if test $? = 0
		then
			echo "file RESULTS/$bname verified identical"
		else
			changed=yes
			echo "file RESULTS/$bname is DIFFERENT to benchmark."
			echo "----------------------------------------------"	>> $LOGFILE
			echo "Differences for file RESULTS/$bname"			>> $LOGFILE
			cat	temp$$.dat								>> $LOGFILE
		fi
	fi
done

if test "$changed" = "no"
then
	echo "--------------------------------------"
	echo "Locally generated version verified against benchmark."
else
	echo "--------------------------------------"
	echo "Differences found.  See $LOGFILE for details."
fi

if test "$missing" = "no"
then
	echo
	echo "All files tested."
else
	echo
	echo "NOT all files tested."
fi

rm	-f temp$$.dat
