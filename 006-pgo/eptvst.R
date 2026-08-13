library(ggplot2)
eptdat <- read.csv("out/eptvst.dat")
head(eptdat)
str(eptdat)
cairo_pdf("out/eptvst.pdf")
eptdat$E <- eptdat$E / eptdat$T
ggplot(data=eptdat,
       mapping = aes(x=T, y=E, color=eptdat$type)) +
       geom_point(size=3) +
       theme(legend.position="none") +
       theme(text=element_text(family="Gentium Book")) +
       xlab("Turns") +
       ylab("EPT")
dev.off()
