library(ggplot2)
pptdat <- read.csv("out/pptvst.dat")
head(pptdat)
str(pptdat)
cairo_pdf("out/pptvst.pdf")
pptdat$P <- pptdat$P / pptdat$T
ggplot(data=pptdat,
       mapping = aes(x=T, y=P, color=atype)) +
       geom_point(size=3) +
       theme(legend.position="none") +
       theme(text=element_text(family="Gentium Book")) +
       xlab("Turns") +
       ylab("PPT")
dev.off()
