#!/bin/sh
#	Download of datasets.
#	uniprot_trembl.dat.gz and others for protein data
#-------------------------------------------------------
echo "The required datasets are available from:-"
echo "... https://ftp.uniprot.org/pub/databases/uniprot/previous_releases/release-2021_03/knowledgebase/"
echo "... Download knowledgebase2021_03.tar.gz"
echo "... NOTE!  this will download a 136GB file."
echo
echo "... When untarred by tar -xvzf - < knowledgebase2021_03.tar.gz"
echo "... amongst others it will produce uniprot_trembl.dat.gz and"
echo	"... and uniprot_sprot.dat.gz."
echo
echo	"... Extract the trembl file by"
echo "... tar -xvzf - < knowledgebase2021_03.tar.gz uniprot_trembl.dat.gz"
echo
echo "... Then rename by prefixing the version to give 21-03_uniprot_trembl.dat.gz"
echo "... It can then be processed by 2-prepareData.sh 21-03 trembl"
echo "... NOTE!  the 2-prepareData.sh stage will create an 89GB file."
