#!/usr/bin/perl
#
#	Reads the .seq files from the original decomposition of SwissProt or
#	TrEMBL and identifies identical proteins, (this will not include PTM).
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#--------------------------------------------------------------
#
use 	strict;
use 	warnings;
use 	Getopt::Long;

my	%opt		= ();

my	$csvfile	= "";

GetOptions(
	'csvfile=s'	=>	\$csvfile,
);

if ( $csvfile eq "" )		{ die "-csvfile mandatory."; }
#
#	Open the csv file for all information IN APPEND mode so they accumulate.
#
open CSV, ">>", $csvfile or die $!;
#
#	Read the seqfile.  This contains all the protein sequences for this species
#	in the format <sequence>,<protein name>
#
my	@partsrec;
while (my $line = <>)
{
	if ( $line =~ /^#/ )	{ next; }
	chomp($line);

	@partsrec	= split( /\,/, $line );

	my	$sequence		= $partsrec[0];
	my	$protname		= $partsrec[1];
	my	$kingdom		= $partsrec[2];
#
#	Extract the species name.
#
	if ( $protname	=~ /([A-Z0-9]+)\_([A-Z0-9]+)/ )
	{
		my	$species	= $2;

		my	$lenprot	= length($sequence);

		printf CSV "%s,%s,%s,%d,%s\n", $sequence, $species, $kingdom, $lenprot, $protname;
	}
	else
	{
		die "Odd protname = $protname.";
	}
}
close CSV;

exit(0);
