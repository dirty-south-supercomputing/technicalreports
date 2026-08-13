library(ggplot2)
dpedat <- read.csv("out/dpevse.dat")
head(dpedat)
str(dpedat)
cairo_pdf("out/dpevse.pdf")
dpedat$P <- dpedat$P / dpedat$E
ggplot(data=dpedat,
       mapping = aes(x=E, y=P, color=atype)) +
       geom_point(size=3) +
       theme(legend.position="none") +
       theme(text=element_text(family="Gentium Book")) +
       xlab("Energy") +
       ylab("PPE")
dev.off()
