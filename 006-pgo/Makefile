.DELETE_ON_ERROR:
.PHONY: all preview clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo foreward story trainer types attacks species pokémon \
		 		stats battle damage unbounded bounded strategy simul max)
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
SPECIES:=out/species
FORMS:=out/forms
SIMUL:=out/simul
HETERO:=out/hetero
TRELS:=out/typerels
FATTACKTEX:=out/fastattacks.tex
CATTACKTEX:=out/chargedattacks.tex
DUALTYPES:=out/dualtypes.tex
SPECIESTEX:=out/species.tex
MEGATEX:=out/mega.tex
PRIMALTEX:=out/primal.tex
DYNATEX:=out/dynas.tex
GIGANTATEX:=out/gigantas.tex
CP1500:=out/cp1500.tex
CP2500:=out/cp2500.tex
HETEROTEX:=out/hetero.tex
TRELSTEX:=out/typerels.tex
GENTEX:=$(DUALTYPES) $(SPECIESTEX) $(MEGATEX) $(CP1500) $(CP2500) \
	$(FATTACKTEX) $(CATTACKTEX) $(HETEROTEX) $(TRELSTEX) $(DYNATEX) \
	$(GIGANTATEX) $(PRIMALTEX)

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

$(OUT)/cp1500.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) 1500 95 > $@

$(OUT)/cp2500.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) 2500 130 > $@

$(OUT)/primal.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) primal > $@

$(OUT)/mega.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) mega > $@

$(OUT)/dynas.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) dynamax > $@

$(OUT)/gigantas.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) gigantamax > $@

$(OUT)/%.tex: $(OUT)/%
	@mkdir -p $(@D)
	$(<) > $@

$(OUT)/%: %.c pgotypes.c
	@mkdir -p $(@D)
	g++ -o $@ -Wall -W -O2 $(<F) -lm

clean:
	@rm -vrf $(TYPES) $(GENTEX) $(SPECIESTEX) $(FATTACKS) $(CATTACKS) $(HETERO) $(TRELS) $(MEGATEX) $(PRIMALTEX)
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
