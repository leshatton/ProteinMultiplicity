#
#	This package uses the methods of Clauset, Shalizi and Newman (2009)
#	to test whether we have a power-law with greater refinement.
#
#	https://stats.stackexchange.com/questions/108843/how-to-test-whether-a-distribution-follows-a-power-law
#-------------------------------------------------------------------
library(poweRlaw)
#
#	For eaxis()
#
library(sfsmisc)

args 	<- commandArgs(trailingOnly = TRUE)
#
#	Defines the range over which to plot the fitted line.
#
x0	<- as.numeric(args[1])
x1	<- as.numeric(args[2])
#
#	Fit a power-law function.  For a exponential tail set
#	the third argument to a reasonable number.
#
TPL=function(par,x,y){
    C=par[1]
    beta=par[2]
    
    est=(C*x^(-beta))
    sum((log(y) - log(est))^2)    
}
#
#	Read in the frequency data from the entire pdf.
#	For example, if we use pdf_c_ti.dat as the raw data, sorted in decreasing size and
#	then annotated with the rank, e.g.
#	1 9475
#	2 9288
#	...
#
#	Note if we were reading in a single column, we would just use
#	data <- scan("c_length_powerlawtest.txt",header=FALSE)
#
data <- read.table("length_powerlawtest.txt",header=FALSE)
#
#	We can plot the pdf just to check it out.
#
#plot(data)
#
#	Define the axes for convenience.
#
x	<- data$V1
y	<- data$V2
#
#	This is a ccdf.  We do not need exponential binning.
#
#	Move the margins over a bit to make room for the y-axis label.
#
postscript(file = "lengths_analysis.eps", horizontal = FALSE, width=6, height=6)
par(mar=c(5,6,4,1)+.1)
#
#	This we can now plot as a log-log.  If its a power-law, it should be straight line.
#	Since we have extracted the y-axis as a density, 1 is the largest possible value.
#
#	NOTE.  The xlim/ylim stuff in the arguments of plot is essential to getting the lines in the right place.
#
plot(x,y,log="xy",pch=16,type="p",cex=2,col="steelblue3",xlab="",ylab="",las=1,xaxt="n",yaxt="n",bty="n",xlim=c(min(x),max(x)),ylim=c(min(y),max(y)))
#
#	Try best fit with initial guesses in par.
#
f	<- optim(par=c(2.0e+07,1.0),TPL,x=x,y=y)
#x1	<- c(seq(0.2,x0,0.1),seq(x0,x1,1))
x1	<- c(seq(x0,x1,1))
y1	<- (f$par[1]*x1^(-f$par[2]))
par(new=T)
plot(x1,y1,log="xy",type="l",lwd=3,col="#CC6666",xlab="",ylab="",las=1,xaxt="n",yaxt="n",xlim=c(min(x),max(x)),ylim=c(min(y),max(y)))
mtext(quote(Rank),1,line=3,cex=1.5)
mtext(quote(Frequency),2,line=4,cex=1.5)
eaxis(1,at=c(1,10,10^2,10^3,10^4),cex.axis=1.5)
eaxis(2,at=c(10^0,10^1,10^2,10^3,10^4,10^5,10^6,10^7),cex.axis=1.5)
#
#	Now we have annotated the axes, we can set a new environment and draw the regression line.
#
dev.off()
