#
#	Copyright Les Hatton, 2013-.
#	Released under the GNU GPL.
#
#	Revision:	$Revision: 1.1 $
#	Date:	$Date: 2019/12/12 14:19:23 $
#-----------------------------------------------------------------------
#
set	term		postscript eps enhanced color dashed
set	output	"VVVV_DDDD_protein_replicas_ccdf_combo.eps"
#
#	Note the set-show aspect of gnuplot.  If you don't show it you
#	don't see it.
#
set	xlabel	'Rank' font "Helvetica,24"
show	xlabel

set	ylabel	'Number of proteins' font "Helvetica,24"
show	ylabel

#set	xrange	[1:8000]
#set	yrange	[0.0001:35000]

set	logscale	xy
set	key	on

#set	title	"TTTT, VVVV, DDDD" font	"Times,24"
#show title

plot 'VVVV_DDDD_protein_replicas_ccdf_combo.dat' index 0 title "Domains of life" with points lw 3,\
	'' index 1 title "plus Viruses" with points lw 3

show tics

#
#	Close the file.
#
set	output
#-----------------------------------------------------------------------
