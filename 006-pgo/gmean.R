library(htmltools)
library(rvest)
library(ggplot2)
args <- commandArgs(trailingOnly=TRUE)
name = args[1]
src <- paste("../../out/gmean", args[2], ".dat.html", sep="")
raw <- read_html(src)
bigtable <- html_element(raw, ".bounded")
df <- as.data.frame(html_table(bigtable))
rec <- subset(df, Form == name)
shadname <- paste(name, ' Shadow', sep="")
bulk <- sqrt(df[["MHP"]] * df[["EffD"]])
ourbulk <- sqrt(rec[["MHP"]] * rec[["EffD"]])
ourcp <- rec[["CP"]]
oureffa <- rec[["EffA"]]
ourgmean <- rec[["Gmean"]]
if(length(ourbulk) == 0){
  quit("no", -1)
}
# will fail if there's no shadow form
shadrec <- subset(df, Form == shadname)
shadbulk <- sqrt(shadrec[["MHP"]] * shadrec[["EffD"]])
shadeffa <- shadrec[["EffA"]]
if(args[2] == "ML"){
  ourconf <- "15-15-15x50"
  ouralpha <- 0.25
}else{
  ourconf <- rec[["IVxL"]]
  ouralpha <- 0.1
}
ename = gsub('[% \\.\']', '', name)
# bulk vs EffA
if(length(shadbulk) == 0){
  # no shadow for this form
  p <- ggplot(data=df, aes(x=bulk, y = EffA, colour=(EffD*MHP*EffA))) +
    geom_point(alpha=ouralpha) +
    geom_point(x=ourbulk, y=oureffa, colour="black", size=3) +
    theme_minimal() +
    theme(legend.position="none",
          text=element_text(family="sans-serif"),
          axis.title=element_text(family="sans-serif")) +
    labs(title=paste(name, " (", ourconf, " CP ", ourcp, ") in ", args[2], sep=""), x="Bulk (sqrt(EffD*MHP))", y="EffA")
  png(paste(paste("graphs/", ename, args[2], sep=""), ".png", sep=""))
  print(p)
  # bulk vs gmean
  p <- ggplot(data=df, aes(x=bulk, y = Gmean, colour=(EffD*MHP*EffA))) +
    geom_point(alpha=ouralpha) +
    geom_point(x=ourbulk, y=ourgmean, colour="black", size=3) +
    theme_minimal() +
    theme(legend.position="none",
          text=element_text(family="sans-serif"),
          axis.title=element_text(family="sans-serif")) +
    labs(title=paste(name, " (", ourconf, " CP ", ourcp, ") in ", args[2], sep=""), x="Bulk (sqrt(EffD*MHP))", y="Gmean")
  png(paste(paste("graphs/", ename, args[2], sep=""), "Gmean.png", sep=""))
  print(p)
}else{
  # this form had a shadow
  p <- ggplot(data=df, aes(x=bulk, y = EffA, colour=(EffD*MHP*EffA))) +
    geom_point(alpha=ouralpha) +
    geom_point(x=ourbulk, y=oureffa, colour="black", size=3) +
    geom_point(x=shadbulk, y=shadeffa, colour="purple", size=3) +
    theme_minimal() +
    theme(legend.position="none",
          text=element_text(family="sans-serif"),
          axis.title=element_text(family="sans-serif")) +
    labs(title=paste(name, " (", ourconf, " CP ", ourcp, ") in ", args[2], sep=""), x="Bulk (sqrt(EffD*MHP))", y="EffA")
  png(paste(paste("graphs/", ename, args[2], sep=""), ".png", sep=""))
  print(p)
  # bulk vs gmean
  p <- ggplot(data=df, aes(x=bulk, y = Gmean, colour=(EffD*MHP*EffA))) +
    geom_point(alpha=ouralpha) +
    geom_point(x=ourbulk, y=ourgmean, colour="black", size=3) +
    geom_point(x=shadbulk, y=ourgmean, colour="purple", size=3) +
    theme_minimal() +
    theme(legend.position="none",
          text=element_text(family="sans-serif"),
          axis.title=element_text(family="sans-serif")) +
    labs(title=paste(name, " (", ourconf, " CP ", ourcp, ") in ", args[2], sep=""), x="Bulk (sqrt(EffD*MHP))", y="Gmean")
  png(paste(paste("graphs/", ename, args[2], sep=""), "Gmean.png", sep=""))
  print(p)
}
