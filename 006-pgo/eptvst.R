library(ggplot2)
data <- read.csv("out/eptvst.dat")
head(data)
str(data)
resfactor=1
cairo_pdf("out/eptvst.pdf")
ept <- data$E / data$T
plot(x=data$T, y=ept,
     xlab="Turns",
     ylab="EPT",
     col=data$type,
     pch=16,
     las=1,
     family="Gentium Book"
     #theme(text=element_text(family="Gentium Book")))
     )
dev.off()
