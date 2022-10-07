#!/bin/sh
#	Create the ccdf for the occurrence of exact replicas.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#-------------------------------------------------
if test $# -ne 2
then
	echo "Usage: $0 [version, e.g. 18-02] [trembl|swissprot]"
	exit
fi

VERSION=$1
DB=$2

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

COMBINED=${VERSION}_${DB}_protein_replicas_ccdf_combo.dat
rm -f $COMBINED

if test ! -f $CSVFILE_SORTED
then
	if test ! -f $CSVFILE
	then
		echo "File $CSVFILE is missing.  Create it with ./2-prepareData.sh"
	fi
#
#	Sort the index on increasing length of protein sequence.
#
	sort -t"," --key=4 $CSVFILE -n | uniq -u				> $CSVFILE_SORTED
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
#
#	Note that we include the zeroth value here.
#
#	We do this for Archaea, Bacteria and Eukaryota and then add viruses.
#
#--------
cat $REPFILE | egrep ",ARCHAEA:|,BACTERIA:|,EUKARYOTA:" | awk '{FS=","}{print $5;}' | sort -n | uniq -c | tail -n +2 \
	| ./normalise ccdf            > $REPLICAS
cat $REPLICAS					>> $COMBINED
#--------
cat $REPFILE | egrep ",ARCHAEA:|,BACTERIA:|,EUKARYOTA:|VIRUSES:" | awk '{FS=","}{print $5;}' | sort -n | uniq -c | tail -n +2 \
	| ./normalise ccdf            > $REPLICAS
cat $REPLICAS					>> $COMBINED

cat	protein_replicas_ccdf_combo.gnu	\
	| sed 's/VVVV/'"${VERSION}"'/g' \
	| sed 's/DDDD/'"${DB}"'/g' \
	| sed 's/TTTT/ALL/g'					> temp$$.gnu
gnuplot	temp$$.gnu

cp	${VERSION}_${DB}_protein_replicas_ccdf_combo.eps				Fig3.eps
#--------
#
#	Clean up.
#
rm	-f temp$$.gnu
