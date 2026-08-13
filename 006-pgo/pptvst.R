library(ggplot2)
pptdat <- read.csv("out/pptvst.dat")
head(pptdat)
str(pptdat)
cairo_pdf("out/pptvst.pdf")
pptdat$P <- pptdat$P / pptdat$T
ggplot(data=pptdat,
       mapping = aes(x=pptdat$T, y=pptdat$P, color=pptdat$type)) +
       geom_point() +
       theme(legend.position="none") +
       theme(text=element_text(family="Gentium Book")) +
       xlab("Turns") +
       ylab("PPT")
dev.off()
