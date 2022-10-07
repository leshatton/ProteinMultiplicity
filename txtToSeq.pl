#!/usr/bin/perl
#
# 	Sieve the Flat files extracting out the sequences and the id.
#
#	Copyright Les Hatton, 2019-.
#	Released under the GNU GPL.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#-----------------------------------------------------------------

use strict;
use warnings;
use Scalar::Util qw(looks_like_number);
use Getopt::Std;

my	(%opt);

my	$infile	= "";
my	$inoc	= 0;
my	$haveoc	= 0;
#-----------------------------------------------------------------
my	$protcount= 0;
my	$id		= "";
#================================================================
#
#	Read the input list, process and and write the output list
#	----------------------------------------------------------

my	$fullseq		= "";
my	$kingdom		= "";
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
while (my $record = <STDIN> )
{
	chop($record);
#
#	This file contains multiple protein records.  Each record is
#	terminated by "^//".
#
	if ( $record =~ /^\/\// )
	{
#
#		End of protein.
#		---------------
#		Not the first one.  Process previous protein.
#
		printf "%s,%s,%s\n", $fullseq, $id, $kingdom;

		$fullseq		= "";

		++$protcount;
	}
	else
	{
#
#		Body of protein.
#		----------------
#		Process the record
#
		if ( $record =~ /^ID/ )
		{
#
#			Parse the id header.
#
			$record	=~ /([A-Z0-9]+_[A-Z0-9]+)/;
			$id		= $1;
		}
		elsif ( $record =~ /^  / )
		{
#
#			Parse the sequence itself.  Strip the spaces and concatenate.
#
			$record	=~ s/[ ]+//g;
			$fullseq	.= $record;
		}
		elsif ( $record =~ /^OC/ )
		{
			if ( $record =~ /Archaea/i )			{ $kingdom	= "ARCHAEA"; }
			elsif ( $record =~ /Eukaryota/i )		{ $kingdom	= "EUKARYOTA"; }
			elsif ( $record =~ /Bacteria/i )		{ $kingdom	= "BACTERIA"; }
			elsif ( $record =~ /Viruses/i )		{ $kingdom	= "VIRUSES"; }
		}
		else
		{
#
#			Ignore the rest.
#
		}
	}
}
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

exit();
#-----------------------------------------------------------------
