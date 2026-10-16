library(ggplot2)
library(ggradar)
library(fmsb)
args <- commandArgs(trailingOnly=TRUE)
name = args[1]
lshort = args[2]
data = read.table("../../out/optstatsradar.r", sep="\t", header=TRUE)
ename = gsub('[ %]', '', name)
png(paste("graphs/", ename, lshort, "Radar.png", sep=""))
rec <- subset(data, Form==paste(name, lshort, sep=""))
#par(mfrow=c(3, 3))
#lapply(1:(nrow(data)/3),
#function(i) {
print(rec)
limits <- data[ data$Form %in% c(paste("Min", lshort, sep=""),paste("Max", lshort, sep="")), ]
print(limits)
radarchart(rbind(limits[1:2, 2:4], rec[2:4]), title=paste(name, lshort), pfcol="black")
dev.off()
