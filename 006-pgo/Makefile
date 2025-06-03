.DELETE_ON_ERROR:
.PHONY: all preview clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo foreward story trainer types attacks species pokémon \
		 		stats damage unbounded bounded strategy simul)
CIRCO:=$(addprefix circo/, $(addsuffix .dot, dragon nature phases rational steel))
DOT:=$(addprefix dot/, $(addsuffix .dot, death jumble))
IMAGES:=$(wildcard images/*)
GRAPHS:=$(addprefix $(OUT)/, $(addsuffix .png, $(CIRCO) $(DOT)))
PDFVIEW:=evince
PDF:=$(addsuffix .pdf, $(PAPERBACK))

UPSTREAM:=$(addsuffix .csv, moves)
CHARGED:=chargedattacks
FAST:=fastattacks
JSON:=$(addprefix $(OUT), $(addsuffix .json, $(CHARGED) $(FAST)))
TYPES:=out/types

preview: all
	$(PDFVIEW) $(PAPERBACK).pdf

all: $(PDF)

$(PAPERBACK).pdf: $(TEX) $(GRAPHS) $(IMAGES) $(JSON) $(PAPERBACK).toc
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(GRAPHS) $(IMAGES) $(JSON)
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(OUT)/dot/%.png: dot/%
	@mkdir -p $(@D)
	dot -Tpng $< > $@

$(OUT)/circo/%.png: circo/%
	@mkdir -p $(@D)
	circo -Tpng $< > $@

$(JSON): csvs $(UPSTREAM) $(TYPES)
	./$(<F)

$(TYPES): types.c
	@mkdir -p $(@D)
	gcc -o $@ -Wall -W -O2 $(<F) -lm

clean:
	@rm -vrf $(JSON) $(TYPES)
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
