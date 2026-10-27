library(htmltools)
library(rvest)
library(ggplot2)
args <- commandArgs(trailingOnly=TRUE)
name = args[1]
src <- paste(paste("../../out/gmean", args[2], sep=""), ".html", sep="")
raw <- read_html(src)
bigtable <- html_element(raw, ".bounded")
df <- as.data.frame(html_table(bigtable))
rec <- subset(df, Form == name)
bulk <- sqrt(df[["MHP"]] * df[["EffD"]])
ourbulk <- sqrt(rec[["MHP"]] * rec[["EffD"]])
ourcp <- rec[["CP"]]
oureffa <- rec[["EffA"]]
ourgmean <- rec[["Gmean"]]
if(length(ourbulk) == 0){
  quit("no", -1)
}
if(args[2] == "ML"){
  ourconf <- "15-15-15x50"
  ouralpha <- 0.25
}else{
  ourconf <- rec[["IVxL"]]
  ouralpha <- 0.1
}
ename = gsub('[% \']', '', name)
p <- ggplot(data=df, aes(x=bulk, y = EffA, colour=(EffD*MHP*EffA))) +
  geom_point(alpha=ouralpha) +
  geom_point(x=ourbulk, y=oureffa, colour="black", size=2) +
  theme_minimal() + 
  theme(legend.position="none",
        text=element_text(family="sans-serif"),
        axis.title=element_text(family="sans-serif")) +
  labs(title=paste(name, " (", ourconf, " CP ", ourcp, ") in ", args[2], sep=""), x="Bulk (sqrt(EffD*MHP))", y="EffA")
png(paste(paste("graphs/", ename, args[2], sep=""), ".png", sep=""))
print(p)
p <- ggplot(data=df, aes(x=bulk, y = Gmean, colour=(EffD*MHP*EffA))) +
  geom_point(alpha=ouralpha) +
  geom_point(x=ourbulk, y=ourgmean, colour="black", size=2) +
  theme_minimal() + 
  theme(legend.position="none",
        text=element_text(family="sans-serif"),
        axis.title=element_text(family="sans-serif")) +
  labs(title=paste(name, " (", ourconf, " CP ", ourcp, ") in ", args[2], sep=""), x="Bulk (sqrt(EffD*MHP))", y="Gmean")
png(paste(paste("graphs/", ename, args[2], sep=""), "Gmean.png", sep=""))
print(p)
