#!/bin/sh
#
#	Standard environment check according to:-
#		https://arxiv.org/abs/1608.06897
#
#	Checks the environment.  This is not a complete check but
#	will currently cover the basics.
#
#	Copyright Les Hatton, 2013-.
#	Released under the GNU GPL.
#
#	Revision:	$Revision: 1.3 $
#	Date:	$Date: 2019/12/12 14:26:59 $
#-----------------------------------------------------------
status=0

which awk			2> /dev/null
if test $? -ne 0
then
	echo "awk MISSING"
	status=1
fi

which convert		2> /dev/null
if test $? -ne 0
then
	echo "convert MISSING"
	status=1
fi

which gcc		2> /dev/null
if test $? -ne 0
then
	echo "gcc MISSING"
	status=1
fi

which gnuplot		2> /dev/null
if test $? -ne 0
then
	echo "gnuplot MISSING"
	status=1
fi

which gunzip		2> /dev/null
if test $? -ne 0
then
	echo "gunzip MISSING"
	status=1
fi

which gzip		2> /dev/null
if test $? -ne 0
then
	echo "gzip MISSING"
	status=1
fi

which make		2> /dev/null
if test $? -ne 0
then
	echo "make MISSING"
	status=1
fi

which perl		2> /dev/null
if test $? -ne 0
then
	echo "perl MISSING"
	status=1
fi

which R			2> /dev/null
if test $? -ne 0
then
	echo "R MISSING"
	status=1
fi

which Rscript		2> /dev/null
if test $? -ne 0
then
	echo "Rscript MISSING"
	status=1
fi

which unzip		2> /dev/null
if test $? -ne 0
then
	echo "unzip MISSING"
	status=1
fi

echo

if test "$status" = "0"
then
	echo "Environment checked.  Everything appears to be there."
	echo "Note that you still need to check if necessary R packages"
	echo "are installed using install.packages() (poweRlaw, methods, sfsmisc and pracma)."
	echo "You should do this now as later scripts"
	echo "require their presence."
else
	echo "Environment checked.  MISSING components, please install before continuing."

	exit
fi

echo "=================================================="
echo "You will now be given the option of building the arithmetic"
echo "check, ES paranoia."
echo
echo "Do you wish to run ES paranoia to check your computer arithmetic: (y or n) ?: "
read dummy
if test "$dummy" = "y"
then
	./make_esp.sh
	./esp32		> RESULTS/esp32.dat
	./esp64		> RESULTS/esp64.dat
	echo "Check RESULTS/esp32.dat RESULTS/esp64.dat for details of your arithmetic check."
else
	echo "You declined to run the arithmetic check.  You can continue but it is"
	echo "recommended you check it at some stage."
	echo "=================================================="
fi
