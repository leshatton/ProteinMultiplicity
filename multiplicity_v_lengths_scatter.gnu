#
#	Specific gnuplot file.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2013/11/18 15:21:10 $
#-----------------------------------------------------------------------
#
set	term		postscript eps enhanced monochrome dashed
set	output	"multiplicity_v_lengths_scatter.eps"
#
#	Note the set-show aspect of gnuplot.  If you don't show it you
#	don't see it.
#
set	xlabel	'Length (amino acids)'		font  "Helvetica,24"
show	xlabel

set	ylabel	'Multiplicity'				font  "Helvetica,24"
show	ylabel

set	xrange	[1:5000]

unset		mxtics
unset		mytics

set	key	off

plot 'multiplicity_v_lengths_scatter.tx2' index 0 with points lw 3

show tics

#
#	Close the file.
#
set	output
#-----------------------------------------------------------------------
