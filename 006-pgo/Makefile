.DELETE_ON_ERROR:
.PHONY: all preview clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo foreward story trainer types attacks species pokémon \
		 		battle unbounded bounded strategy simul max spawn example about)
CIRCO:=$(addprefix circo/, $(addsuffix .dot, dragon nature phases rational steel))
DOT:=$(addprefix dot/, $(addsuffix .dot, death jumble))
PDFVIEW:=okular
PDF:=$(addsuffix .pdf, $(PAPERBACK))

UPSTREAM:=$(addsuffix .csv, moves)
FATTACKS:=out/fastattacks
CATTACKS:=out/chargedattacks
DUALTYPES:=out/dualtypes
SPECIES:=out/species
FORMS:=out/forms
SIMUL:=out/simul
HETERO:=out/hetero
TRELS:=out/typerels
USERS:=out/users
WEATHER:=out/weather
POPS:=out/populations
DPEVSE:=out/dpevse
PPTVST:=out/pptvst
EPTVST:=out/eptvst
EVOLUTIONS:=out/evolutions
ATKTYPES:=out/attacktypes
SINDEX:=$(OUT)/sindex
FATTACKTEX:=out/fastattacks.tex
CATTACKTEX:=out/chargedattacks.tex
DUALTYPESTEX:=out/dualtypes.tex
DPEVSEDAT:=$(DPEVSE).dat
PPTVSTDAT:=$(PPTVST).dat
EPTVSTDAT:=$(EPTVST).dat
DUALSUMSTEX:=out/dualsummaries.tex
FUSERSTEX:=$(OUT)/fastusers.tex
CUSERSTEX:=$(OUT)/chargedusers.tex
ATKTYPESTEX:=out/attacktypes.tex
SPECIESTEX:=out/species.tex
EVOLUTIONSTEX:=$(EVOLUTIONS).tex
MEGATEX:=out/mega.tex
FUSEDTEX:=$(OUT)/fused.tex
PRIMALTEX:=out/primal.tex
DYNATEX:=out/dynas.tex
CROWNEDTEX:=out/crowned.tex
SINDEXTEX:=$(SINDEX).tex
GIGANTATEX:=out/gigantas.tex
WEATHERTEX:=$(WEATHER).tex
POPSTEX:=$(POPS).tex
CP1500:=out/cp1500.tex
CP2500:=out/cp2500.tex
HETEROTEX:=out/hetero.tex
TRELSTEX:=out/typerels.tex
GENTEX:=$(DUALTYPESTEX) $(SPECIESTEX) $(MEGATEX) $(CP1500) $(CP2500) \
	$(FATTACKTEX) $(CATTACKTEX) $(HETEROTEX) $(TRELSTEX) $(DYNATEX) \
	$(GIGANTATEX) $(PRIMALTEX) $(CROWNEDTEX) $(ATKTYPESTEX) $(WEATHERTEX) \
	$(CUSERSTEX) $(FUSERSTEX) $(DUALSUMSTEX) $(POPSTEX) $(EVOLUTIONSTEX) \
	$(FUSEDTEX) $(SINDEXTEX) $(DPEVSEDAT) $(PPTVSTDAT) $(EPTVSTDAT)
GRAPHS:=$(addprefix $(OUT)/, $(addsuffix .png, $(CIRCO) $(DOT))) \
	octave/greninja.png

preview: all
	$(PDFVIEW) $(PAPERBACK).pdf

all: $(PDF) $(SIMUL)

$(PAPERBACK).pdf: $(TEX) $(GRAPHS) $(GENTEX) $(PAPERBACK).toc
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(GRAPHS) $(GENTEX)
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(OUT)/dot/%.png: dot/%
	@mkdir -p $(@D)
	dot -Tpng $< > $@

$(OUT)/circo/%.png: circo/%
	@mkdir -p $(@D)
	circo -Tpng $< > $@

$(OUT)/dualtypes.tex: $(OUT)/dualtypes
	@mkdir -p $(@D)
	$(<) > $@

$(OUT)/dualsummaries.tex: $(OUT)/dualtypes
	@mkdir -p $(@D)
	$(<) summary > $@

$(OUT)/cp1500.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) 1500 95 > $@

$(OUT)/cp2500.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) 2500 130 > $@

$(CUSERSTEX): $(OUT)/users
	@mkdir -p $(@D)
	$(<) charged > $@

$(FUSERSTEX): $(OUT)/users
	@mkdir -p $(@D)
	$(<) fast > $@

$(OUT)/primal.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) primal > $@

$(OUT)/fused.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) fused > $@

$(OUT)/mega.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) mega > $@

$(OUT)/crowned.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) crowned > $@

$(OUT)/dynas.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) dynamax > $@

$(OUT)/gigantas.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) gigantamax > $@

$(OUT)/graphs/%.png: $(OUT)/graphs/%.plg
	@mkdir -p $(@D)
	gnuplot $(<) > $@

$(OUT)/%.tex: $(OUT)/%
	@mkdir -p $(@D)
	$(<) > $@

# FIXME need to graph the output and generate image
$(OUT)/%.dat: $(OUT)/%
	@mkdir -p $(@D)
	$(<) > $@

$(OUT)/%: %.cpp pgotypes.cpp
	@mkdir -p $(@D)
	g++ -std=c++23 -o $@ -Wall -W -O2 $(<F) -lm

clean:
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
