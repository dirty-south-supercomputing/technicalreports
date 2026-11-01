library(htmltools)
library(rvest)
library(ggplot2)

args <- commandArgs(trailingOnly=TRUE)
src <- paste(paste("../../out/effa", args[1], sep=""), ".html", sep="")
raw <- read_html(src)
df <- as.data.frame(html_table(raw))
p <- ggplot(df) +
  geom_segment(data=df, aes(x=Low, y=Form, yend=Form, xend=High), linewidth=4) +
  geom_point(aes(x=Low, y=Form), size=4) +
  geom_point(aes(x=High, y=Form), size=4)
png(paste(paste("./effa", args[1], sep=""), ".png", sep=""), width=480, height=18000)
print(p)
