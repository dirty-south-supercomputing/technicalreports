library(ggplot2)
library(ggradar)
library(fmsb)
data = read.table("out/optstatsradar.r", sep="\t", header=TRUE)
cairo_pdf("out/optstatsradar.pdf")
par(mfrow=c(3, 3))
#lapply(1:(nrow(data)/3), function(i) { ggradar(data[(3*i-2):(3*i), 1:4], grid.max=250, grid.mid=250, plot.title=data[3*i,1]) })
lapply(1:(nrow(data)/3),
function(i) {
        #radarchart(rbind(data[1:2, 2:4], data[(3*i):(3*i+2), 2:4]), title=data[3*i, 1], pfcol="black")
        radarchart(rbind(data[1:2, 2:4], data[(3*i+2):(3*i+2), 2:4]), title=data[3*i+2, 1], pfcol="black")
})
dev.off()
