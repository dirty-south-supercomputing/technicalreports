library(htmltools)
library(rvest)
library(ggplot2)
args <- commandArgs(trailingOnly=TRUE)
name = args[1]
src <- paste(paste("out/gmean", args[2], sep=""), ".html", sep="")
raw <- read_html(src)
df <- as.data.frame(html_table(raw))
rec <- subset(df, Form == name)
bulk <- sqrt(df[["MHP"]] * df[["EffD"]])
ourbulk <- sqrt(rec[["MHP"]] * rec[["EffD"]])
ourgmean <- rec[["Gmean"]]
p <- ggplot(data=df, aes(x=bulk, y = Gmean)) +
  geom_point(colour="grey") +
  geom_point(aes(x=ourbulk, y=ourgmean), colour="blue") +
  theme_minimal() + 
  labs(title=paste(name, "in", args[2]), x="Bulk (sqrt(EffD*MHP))", y="cbrt(Base product)")
print(name)
png(paste(paste("out/graphs/", name, args[2], sep=""), ".png", sep=""))
print(p)
dev.off()
