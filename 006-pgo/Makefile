.DELETE_ON_ERROR:
.PHONY: all preview testsimul clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo foreward objectives trainer friends types attacks \
											 species pokémon battle unbounded bounded tactics \
											 simul spawn example about ooo optimal notice)
CIRCO:=$(addprefix circo/, $(addsuffix .dot, dragon nature phases rational steel))
DOT:=$(addprefix dot/, $(addsuffix .dot, death jumble))
PDFVIEW:=okular
PDF:=$(addsuffix .pdf, $(PAPERBACK))
MAKEFILE:=Makefile

UPSTREAM:=$(addsuffix .csv, moves)
BREAKPOINTS:=out/breakpoints
SIMUL:=out/s/simul
FATTACKS:=out/fastattacks
CARDEXAMPLE:=out/cardexample
CATTACKS:=out/chargedattacks
DUALTYPES:=out/dualtypes
SPECIES:=out/species
FORMS:=out/forms
HETERO:=out/hetero
TRELS:=out/typerels
TIMETOFIRST:=out/timetofirst
USERS:=out/users
REGIONS:=out/regions
WEATHER:=out/weather
POPS:=out/populations
DPEVSE:=out/dpevse
PPTVST:=out/pptvst
PAIRS:=out/pairs
EPTVST:=out/eptvst
ATKTYPES:=out/attacktypes
SINDEX:=$(OUT)/sindex
FATTACKTEX:=out/fastattacks.tex
CATTACKTEX:=out/chargedattacks.tex
CARDEXAMPLETEX:=$(CARDEXAMPLE).tex
DUALTYPESTEX:=out/dualtypes.tex
DPEVSEDAT:=$(DPEVSE).dat
PPTVSTDAT:=$(PPTVST).dat
EPTVSTDAT:=$(EPTVST).dat
DUALSUMSTEX:=out/dualsummaries.tex
FUSERSTEX:=$(OUT)/fastusers.tex
CUSERSTEX:=$(OUT)/chargedusers.tex
ATKTYPESTEX:=out/attacktypes.tex
SPECIESTEX:=out/species.tex
FPAIRSTEX:=$(OUT)/fpairs.tex
CPAIRSTEX:=$(OUT)/cpairs.tex
MEGATEX:=out/mega.tex
#DYNATEX:=out/dynas.tex
SINDEXTEX:=$(SINDEX).tex
GIGANTATEX:=out/gigantas.tex
WEATHERTEX:=$(WEATHER).tex
POPSTEX:=$(POPS).tex
REGIONSTEX:=$(REGIONS).tex
CP1500A:=out/cp1500a.tex
CP2500A:=out/cp2500a.tex
CP1500G:=out/cp1500g.tex
CP2500G:=out/cp2500g.tex
HETEROTEX:=out/hetero.tex
TRELSTEX:=out/typerels.tex
TIMETOFIRSTTEX:=$(TIMETOFIRST).tex
BREAKPOINTSDAT:=$(BREAKPOINTS).tex
GENTEX:=$(DUALTYPESTEX) $(SPECIESTEX) $(MEGATEX) $(CP1500A) $(CP2500A) \
	$(FATTACKTEX) $(CATTACKTEX) $(HETEROTEX) $(TRELSTEX) \
	$(GIGANTATEX) $(ATKTYPESTEX) $(WEATHERTEX) $(REGIONSTEX) \
	$(CUSERSTEX) $(FUSERSTEX) $(DUALSUMSTEX) $(POPSTEX) $(CPAIRSTEX) \
	$(SINDEXTEX) $(DPEVSEDAT) $(PPTVSTDAT) $(EPTVSTDAT) \
	$(BREAKPOINTSDAT) $(FPAIRSTEX) $(CARDEXAMPLETEX) $(CP1500G) $(CP2500G) \
	$(TIMETOFIRSTTEX)
GRAPHS:=$(addprefix $(OUT)/, $(addsuffix .png, $(CIRCO) $(DOT))) \
	octave/greninja.png
SIMULSRC:=$(addprefix s/, simul.cpp bottom.h charged.h ko.h moves.h sift.h simul.h top.h)

preview: all
	$(PDFVIEW) $(PAPERBACK).pdf

all: $(PDF) $(SIMUL)

$(PAPERBACK).pdf: $(TEX) $(MAKEFILE) $(GRAPHS) $(GENTEX) $(SIMULSRC) $(PAPERBACK).toc
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(MAKEFILE) $(GRAPHS) $(GENTEX) $(SIMULSRC)
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

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

$(OUT)/cp1500a.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) a 1500 128.94 > $@

$(OUT)/cp1500g.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) g 1500 124.66 > $@

$(OUT)/cp2500a.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) a 2500 161.98 > $@

$(OUT)/cp2500g.tex: $(OUT)/bounded
	@mkdir -p $(@D)
	$(<) g 2500 156.80 > $@

$(CUSERSTEX): $(OUT)/users
	@mkdir -p $(@D)
	$(<) charged > $@

$(FUSERSTEX): $(OUT)/users
	@mkdir -p $(@D)
	$(<) fast > $@

$(OUT)/mega.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) mega > $@

#$(OUT)/dynas.tex: $(OUT)/forms
#	@mkdir -p $(@D)
#	$(<) dynamax > $@

$(OUT)/gigantas.tex: $(OUT)/forms
	@mkdir -p $(@D)
	$(<) gigantamax > $@

$(FPAIRSTEX): $(PAIRS)
	@mkdir -p $(@D)
	$(<) fast > $@

$(CPAIRSTEX): $(PAIRS)
	@mkdir -p $(@D)
	$(<) charged > $@

$(OUT)/graphs/%.png: $(OUT)/graphs/%.plg
	@mkdir -p $(@D)
	gnuplot $(<) > $@

$(OUT)/%.tex: $(OUT)/%
	@mkdir -p $(@D)
	$(<) > $@

$(BREAKPOINTSDAT): $(basename $(BREAKPOINTS))
	@mkdir -p $(@D)
	$(<) clodsire 15-15-15@99 "poison sting" "megahorn/earthquake" sawk 15-14-14@99 "low kick" "focus blast/low sweep" > $@

$(OUT)/%.dat: $(OUT)/%
	@mkdir -p $(@D)
	$(<) > $@

$(SIMUL): $(SIMULSRC) pgotypes.cpp
	@mkdir -p $(@D)
	g++ -std=c++23 -march=native -mtune=native -o $@ -Wall -W -O3 $(<) -lm

$(OUT)/%: %.cpp pgotypes.cpp
	@mkdir -p $(@D)
	g++ -std=c++23 -g -o $@ -Wall -W -O2 $(<F) -lm

testsimul: $(SIMUL)
	time $(<) clodsire 15-15-15@70 "poison sting" "megahorn" sawk 15-14-14@50 "low kick" "focus blast"
	time $(<) clodsire 15-15-15@70 "poison sting" "earthquake" sawk 15-14-14@50 "low kick" "low sweep"
	time $(<) clodsire 15-15-15@50 "poison sting" "megahorn/earthquake" sawk 15-14-14@50 "low kick" "focus blast/low sweep"
	time $(<) clodsire 15-15-15@99 "poison sting" "megahorn/earthquake" sawk 15-14-14@99 "low kick" "focus blast/low sweep"

clean:
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof $(PAPERBACK).listing
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
	@rm -vrf svg-inkscape
	@rm -vrf _minted
	@rm -vf tags
