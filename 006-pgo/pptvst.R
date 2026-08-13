library(ggplot2)
data <- read.csv("out/pptvst.dat")
head(data)
str(data)
png("out/pptvst.png")
ppt <- data$P / data$T
plot(x=data$T, y=ppt,
     xlab="Turns",
     ylab="PPT",
     col=data$type,
     pch=16,
     las=1,
     family="Gentium Book"
     #theme(text=element_text(family="Gentium Book")))
     )
dev.off()
