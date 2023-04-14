#!/bin/sh
#	Create the CSV file in the form <protein>,<species>,<kingdom>,<length>
#	from the reduced Fasta dataset.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#-------------------------------------------------
if test $# -ne 2
then
	echo "Usage: $0 [version, e.g. 18-02] [trembl|sprot]"
	exit
fi

VERSION=$1
DB=$2

if test "$DB" = "trembl"
then
	SEQFILE=${VERSION}_uniprot_trembl.dat.gz
	CSVFILE=${VERSION}_trembl_protein_index.csv
elif test "$DB" = "swissprot"
then
	SEQFILE=${VERSION}_uniprot_sprot.dat.gz
	CSVFILE=${VERSION}_swissprot_protein_index.csv
else
	echo "Unknown database source, $DB"
	exit
fi

if test ! -f $SEQFILE
then
	echo "$SEQFILE not present."
	exit
fi

rm -f $CSVFILE
#
#	Note that CSVFILE appends here.
#
zcat	$SEQFILE	| ./txtToSeq.pl | ./createProteinCSVFasta.pl	-csvfile $CSVFILE

echo "$CSVFILE created."
#
#	Finally, we will need the normaliser and the fast sequence comparator.
#
if test ! -f normalise
then
	cc -o normalise normalise.c
fi

if test ! -f fastSeqComparison
then
	cc -o fastSeqComparison fastSeqComparison.c
fi

echo "You may now continue with the processing stages, 3-xxxx.sh for each figure"
echo "Process them in order 3-fig1.sh, 3-fig2.sh, 3-fig3.sh, 3-fig4.sh, "
echo "3-Rscripts_fig1.sh, 3-Rscripts_fig2.sh"
