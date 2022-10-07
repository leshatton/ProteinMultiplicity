#!/usr/bin/perl
#
#	Reads a SORTED .csv file for all proteins to find a protein / species
#	relationship.  The .csv file must be sorted in increasing order of
#	protein length.  sort is multi-threaded and uses memory very well making
#	this step do-able without much memory demand.  The SORTED file must also
#	have duplicates removed with uniq -u.
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
my	$repfile	= "";

GetOptions(
	'csvfile=s'	=>	\$csvfile,
	'nosing'		=>	\$opt{n},
	'repfile=s'	=>	\$repfile,
);

if ( $csvfile eq "" )		{ die "-csvfile mandatory."; }
if ( $repfile eq "" )		{ die "-repfile mandatory."; }
#
#	Open the csv file for all information.
#
open REP, ">>", $repfile or die $!;
#
#	Read the csvfile.  This contains all the protein sequences for all species.
#
my	%repspecies		= ();
my	%repspecies_len	= ();
my	%repspecies_flags	= ();
my	%repspecies_count	= ();
my	%repspecies_pname	= ();

my	$last_len			= 0;
#
#	Kingdom flags
#
my	$FLAG_ARCHAEA		= 1;
my	$FLAG_BACTERIA		= 2;
my	$FLAG_EUKARYOTA	= 4;
my	$FLAG_VIRUSES		= 8;

open CSV, "<", $csvfile or die $!;
while (my $line = <CSV>)
{
	if ( $line =~ /^#/ )	{ next; }
	chomp($line);
	my	@fields	= split(/,/, $line);

	my	$seq		= $fields[0];
	my	$spec	= $fields[1];
	my	$king	= $fields[2];
	my	$len		= $fields[3];
	my	$prot	= $fields[4];

	my	$categ	= "";
	if ( defined $prot )				{ $categ	= $king . ':' . $prot; }
	else								{ $categ  = $king . ':' . $spec; }

	my	$kflags	= 0;

	if ( $king eq "ARCHAEA" )			{ $kflags	|= $FLAG_ARCHAEA; }
	elsif ( $king eq "BACTERIA" )			{ $kflags	|= $FLAG_BACTERIA; }
	elsif ( $king eq "EUKARYOTA" )		{ $kflags	|= $FLAG_EUKARYOTA; }
	elsif ( $king eq "VIRUSES" )			{ $kflags	|= $FLAG_VIRUSES; }

	my	$nspecs	= 0;

	if ( $len < $last_len )		{ die "Input csv file not sorted on increasing length len=$len, last_len=$last_len."; }
#
#	If there is an increase in length, we output the previous category entries as there can
#	be no more matching this sequence.
#
	if ( $len > $last_len )
	{
#
#		A new length category has started.  Output the existing ones.
#
		for my $outseq (sort keys %repspecies)
		{
			my	$multiking	= '';
			if ( $repspecies_flags{$outseq} & $FLAG_ARCHAEA )			{ $multiking	.= '*'; }
			if ( $repspecies_flags{$outseq} & $FLAG_BACTERIA )		{ $multiking	.= '*'; }
			if ( $repspecies_flags{$outseq} & $FLAG_EUKARYOTA )		{ $multiking	.= '*'; }
			if ( $repspecies_flags{$outseq} & $FLAG_VIRUSES )			{ $multiking	.= '*'; }

			if ( ! ( defined $opt{n} && ($repspecies_count{$outseq} == 1) ) )
			{
				printf REP "%d,%s,%s,%s,%d\n", 	$repspecies_len{$outseq}, 
											$multiking, 
											$outseq, 
											$repspecies{$outseq},
											$repspecies_count{$outseq};

			}
		}
#
#		And reset the accumulators to preserve memory.
#
		undef %repspecies;
		undef %repspecies_len;
		undef %repspecies_flags;
		undef %repspecies_count;
		undef %repspecies_pname;

		%repspecies			= ();
		%repspecies_len		= ();
		%repspecies_flags		= ();
		%repspecies_count		= ();
		%repspecies_pname		= ();
#
#		Store this first example of the new length and reset last length.
#
		$repspecies{$seq}		= $categ;
		$repspecies_len{$seq}	= $len;
		$repspecies_flags{$seq}	= $kflags;
		$repspecies_count{$seq}	= 1;

		$last_len	= $len;
	}
	else
	{
#
#		Same lengths, just store.
#
		if ( defined $repspecies{$seq} )
		{
#
#			Duplicates have already been removed.
#
			$repspecies{$seq}		.= '/' . $categ;
			$repspecies_flags{$seq}	|= $kflags;
			++$repspecies_count{$seq};
		}
		else
		{
			$repspecies{$seq}		= $categ;
			$repspecies_flags{$seq}	= $kflags;
			$repspecies_count{$seq}	= 1;
		}

		$repspecies_len{$seq}	= $len;
		$repspecies_pname{$seq}	= $prot;
	}
}
#
#	Flush any remaining.
#
for my $outseq (sort keys %repspecies)
{
	my	$multiking	= '';
	if ( $repspecies_flags{$outseq} & $FLAG_ARCHAEA )			{ $multiking	.= '*'; }
	if ( $repspecies_flags{$outseq} & $FLAG_BACTERIA )		{ $multiking	.= '*'; }
	if ( $repspecies_flags{$outseq} & $FLAG_EUKARYOTA )		{ $multiking	.= '*'; }
	if ( $repspecies_flags{$outseq} & $FLAG_VIRUSES )			{ $multiking	.= '*'; }

	if ( ! ( defined $opt{n} && ($repspecies_count{$outseq} == 1) ) )
	{
		printf REP "%d,%s,%s,%s,%d\n", 	$repspecies_len{$outseq}, 
									$multiking, 
									$outseq, 
									$repspecies{$outseq},
									$repspecies_count{$outseq};
	}
}

close CSV;
close REP;

exit(0);
