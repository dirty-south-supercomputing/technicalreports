.DELETE_ON_ERROR:
.PHONY: all preview testsimul clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo foreward objectives trainer friends types attacks \
											 species pokémon battle unbounded bounded strategy \
											 simul max spawn example about ooo)
CIRCO:=$(addprefix circo/, $(addsuffix .dot, dragon nature phases rational steel))
DOT:=$(addprefix dot/, $(addsuffix .dot, death jumble))
PDFVIEW:=okular
PDF:=$(addsuffix .pdf, $(PAPERBACK))

UPSTREAM:=$(addsuffix .csv, moves)
BREAKPOINTS:=out/breakpoints
SIMUL:=out/s/simul
FATTACKS:=out/fastattacks
CATTACKS:=out/chargedattacks
DUALTYPES:=out/dualtypes
SPECIES:=out/species
FORMS:=out/forms
HETERO:=out/hetero
TRELS:=out/typerels
USERS:=out/users
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
FUSEDTEX:=$(OUT)/fused.tex
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
BREAKPOINTSDAT:=$(BREAKPOINTS).tex
GENTEX:=$(DUALTYPESTEX) $(SPECIESTEX) $(MEGATEX) $(CP1500) $(CP2500) \
	$(FATTACKTEX) $(CATTACKTEX) $(HETEROTEX) $(TRELSTEX) $(DYNATEX) \
	$(GIGANTATEX) $(CROWNEDTEX) $(ATKTYPESTEX) $(WEATHERTEX) \
	$(CUSERSTEX) $(FUSERSTEX) $(DUALSUMSTEX) $(POPSTEX) $(CPAIRSTEX) \
	$(FUSEDTEX) $(SINDEXTEX) $(DPEVSEDAT) $(PPTVSTDAT) $(EPTVSTDAT) \
	$(BREAKPOINTSDAT) $(FPAIRSTEX)
GRAPHS:=$(addprefix $(OUT)/, $(addsuffix .png, $(CIRCO) $(DOT))) \
	octave/greninja.png
SIMULSRC:=$(addprefix s/, simul.cpp bottom.h charged.h ko.h moves.h sift.h simul.h top.h)

preview: all
	$(PDFVIEW) $(PAPERBACK).pdf

all: $(PDF) $(SIMUL)

$(PAPERBACK).pdf: $(TEX) $(GRAPHS) $(GENTEX) $(SIMULSRC) $(PAPERBACK).toc
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(GRAPHS) $(GENTEX) $(SIMULSRC)
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

# FIXME need to graph the output and generate image
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
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
	@rm -vrf svg-inkscape
