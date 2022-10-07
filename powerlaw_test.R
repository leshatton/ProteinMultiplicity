#
#	This package uses the methods of Clauset, Shalizi and Newman (2009)
#	to test whether we have a power-law with greater refinement.
#
#	https://stats.stackexchange.com/questions/108843/how-to-test-whether-a-distribution-follows-a-power-law
#-------------------------------------------------------------------
library(poweRlaw)
library(methods)
#
#	Bring in interp1.
#
library(pracma)

args 	<- commandArgs(trailingOnly = TRUE)
#
#	Defines the range over which to plot the fitted line.
#
x0	<- as.numeric(args[1])
x1	<- as.numeric(args[2])
nsms	<- as.numeric(args[3])
#
#	For this dataset, its small enough to use the whole thing without exponential
#	binning and is already a rank-ordered ccdf.
#
indexOfLastNonZero <- function(x) {
	ix	= 0
	for(i in 1:length(x)) {
		ix = i
        	if (x[i] == 0) {
			ix = i
               break
        	}
     } 

	return(ix-1)
}
#
#	Read in the frequency data.
#
data <- read.table("length_powerlawtest.txt",header=FALSE)
x    <- data$V1
y    <- data$V2
#
#	The poweRlaw package just takes a vector but it must be as a rank order so evenly
#	sampled in x so we resample it.
#
xr	<- c(seq(min(x),max(x),1))
yr	<- interp1(x, y, xi = xr)
yt	<- yr[1:indexOfLastNonZero(yr)]
#
#	Plot as sanity check.
#
#xt	<- xr[1:length(yt)]
#plot(xt,yt,log="xy")

plobj	<- conpl$new(yt)
#
#	Estimate the xmin and the exponent α of the power law, and assign them to the power law object
#	The estimate_xmin() mechanism is used because we are normally fitting a power-law to the tail.
#	In this case however, the whole distribution looks power-law so we override the xmin with a
#	visual estimate.
#
plobj$xmin	<- x0
est <- estimate_pars(plobj)
plobj$pars <- est$pars
cat("\n")
cat("Power-law parameters, xmin = ", plobj$xmin, ", pars(alpha) = ", plobj$pars, "\n")
#
#	Also, at this point, you can see the KS statistic.  We do not use this as xmin was
#	estimated visually as described above.
#
#cat("\n")
#cat("KS statistic, (max distance between data and power-law distribution", est$KS, "\n")
#
#	KS statistic tells you how well power law distribution fits your data, but it 
#	doesn't tell you how likely your data is drawn from power law. So you also need a p value. 
#	This is how you do it:  (Note, no of sims = 1000 requires several hours).
#
bs <- bootstrap_p(plobj,xmax=max(x),no_of_sims=nsms)
cat("Number of simulations = ", nsms, "\n")
cat("\n")
cat("Estimated p-value = ", bs$p, "\n")
print("(Should be bigger than 0.1 for plausibility).")
print("End Clauset test")
