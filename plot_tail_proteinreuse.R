#
#	Script to plot the tail of the power law distribution and measure its flatness.
#
x <- scan('uni_x.txt')
y <- scan('uni_y.txt')
universe_mine <- data.frame(x=x,y=y)
postscript(file = "rtail.eps", horizontal = FALSE)
plot(universe_mine)
dev.off()
uni_mine=lm(y~x,data=universe_mine)
summary(uni_mine)
