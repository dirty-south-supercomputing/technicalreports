.DELETE_ON_ERROR:
.PHONY: all preview clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo foreward story trainer types attacks species pokémon \
		 		stats battle damage unbounded bounded strategy simul)
CIRCO:=$(addprefix circo/, $(addsuffix .dot, dragon nature phases rational steel))
DOT:=$(addprefix dot/, $(addsuffix .dot, death jumble))
IMAGES:=$(wildcard images/*)
GRAPHS:=$(addprefix $(OUT)/, $(addsuffix .png, $(CIRCO) $(DOT)))
PDFVIEW:=evince
PDF:=$(addsuffix .pdf, $(PAPERBACK))

UPSTREAM:=$(addsuffix .csv, moves)
FATTACKS:=out/fastattacks
CATTACKS:=out/chargedattacks
TYPES:=out/dualtypes
STATS:=out/species
SIMUL:=out/simul
HETERO:=out/hetero
TRELS:=out/typerels
FATTACKTEX:=out/fastattacks.tex
CATTACKTEX:=out/chargedattacks.tex
DUALTYPES:=out/dualtypes.tex
SPECIES:=out/species.tex
BOUNDED:=out/bounded.tex
HETEROTEX:=out/hetero.tex
TRELSTEX:=out/typerels.tex
GENTEX:=$(DUALTYPES) $(SPECIES) $(BOUNDED) $(FATTACKTEX) $(CATTACKTEX) $(HETEROTEX) $(TRELSTEX)

preview: all
	$(PDFVIEW) $(PAPERBACK).pdf

all: $(PDF) $(SIMUL)

$(PAPERBACK).pdf: $(TEX) $(GRAPHS) $(IMAGES) $(GENTEX) $(PAPERBACK).toc
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(GRAPHS) $(IMAGES) $(GENTEX)
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(OUT)/dot/%.png: dot/%
	@mkdir -p $(@D)
	dot -Tpng $< > $@

$(OUT)/circo/%.png: circo/%
	@mkdir -p $(@D)
	circo -Tpng $< > $@

$(OUT)/%.tex: $(OUT)/%
	@mkdir -p $(@D)
	$(<) > $@

$(OUT)/%: %.c pgotypes.c
	@mkdir -p $(@D)
	g++ -o $@ -Wall -W -O2 $(<F) -lm

clean:
	@rm -vrf $(TYPES) $(GENTEX) $(STATS) $(FATTACKS) $(CATTACKS) $(HETERO) $(TRELS)
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
