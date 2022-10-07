#!/bin/sh
#	Create the ccdf for the occurrence of exact replicas.
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
#
#	NOTE!  You may have to point this somewhere else if your root partition
#	is quite small.
#
TMPDIR=/tmp

if test "$DB" = "trembl"
then
	CSVFILE=${VERSION}_${DB}_protein_index.csv
	CSVFILE_SORTED=${VERSION}_${DB}_protein_index_sorted.csv
	REPFILE=${VERSION}_${DB}_protein_sharing.csv
	REPLICAS=${VERSION}_${DB}_protein_replicas_ccdf.dat
	RANALYSIS=${VERSION}_${DB}_protein_replicas.out
elif test "$DB" = "swissprot"
then
	CSVFILE=${VERSION}_${DB}_protein_index.csv
	CSVFILE_SORTED=${VERSION}_${DB}_protein_index_sorted.csv
	REPFILE=${VERSION}_${DB}_protein_sharing.csv
	REPLICAS=${VERSION}_${DB}_protein_replicas_ccdf.dat
	RANALYSIS=${VERSION}_${DB}_protein_replicas.out
else
	echo "Unknown database source, $DB"
	exit
fi

if test ! -f $CSVFILE_SORTED
then
	if test ! -f $CSVFILE
	then
		echo "File $CSVFILE is missing.  Create it with ./2-prepareData.sh"
		exit
	fi
#
#	Sort the index on increasing length of protein sequence.
#
	sort -t"," --key=4 $CSVFILE -n -T $TEMPDIR | uniq -u				> $CSVFILE_SORTED
	rm -f $CSVFILE
else
	echo "File $CSVFILE_SORTED already exists.  To recreate, delete it and rerun $0."
fi
#
#	Find all the shared proteins.
#
#	Format is <length>,*-**** (number of kingdoms),<sequence>,<kingdom/species strings>,<count>,<protein name>
#
#	Create the whole thing.
#
if test ! -f $REPFILE
then
	./replicasInSpecies.pl -csvfile $CSVFILE_SORTED -repfile $REPFILE
else
	echo "File $REPFILE already exists.  To recreate, delete it and re-run $0."
fi
#
#	Create the ccdf for the replica occurrence.  We get rid of the first line with
#	tail because that count might be turned off in replicasInSpecies.pl.
#	Note that we include the zeroth value by specifying normalise ccdf0.
#
#	We do this for each individual DL+V and then the whole.
#--------
echo "Creating Re-use distribution for ALL ..."		>> $RANALYSIS
cat $REPFILE | awk '{FS=","}{print $5;}' | sort -n | uniq -c | tail -n +2 \
	| ./normalise ccdf            > $REPLICAS

cat	protein_replicas_ccdf.gnu	\
	| sed 's/VVVV/'"${VERSION}"'/g' \
	| sed 's/DDDD/'"${DB}"'/g' \
	| sed 's/TTTT/ALL/g'						> temp$$.gnu
gnuplot	temp$$.gnu
mv	${VERSION}_${DB}_protein_replicas_ccdf.eps		${VERSION}_${DB}_protein_replicas_ccdf_all.eps
cp	${VERSION}_${DB}_protein_replicas_ccdf_all.eps	Fig1.eps
#
#	Clean up.
#
rm	-f temp$$.gnu
