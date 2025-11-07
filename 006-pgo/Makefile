.DELETE_ON_ERROR:
.PHONY: all preview webpage testsimul clean test testdb testsimullong
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo foreward objectives trainer friends types attacks \
											 species pokémon battle unbounded bounded tactics \
											 simul spawn example about ooo optimal notice timespace \
											 speciescards attackcards feraligatr2500 items sresearch)
CIRCO:=$(addprefix circo/, $(addsuffix .dot, dragon nature phases rational steel))
DOT:=$(addprefix dot/, $(addsuffix .dot, death jumble))
PDFVIEW:=okular
PDF:=$(addsuffix .pdf, $(PAPERBACK))
MAKEFILE:=Makefile

BIGLISTS:=$(addsuffix .pdf, listGL listUL listML listTTF)

UPSTREAM:=$(addsuffix .csv, moves)
TESTDB:=$(OUT)/testdb
BREAKPOINTS:=$(OUT)/breakpoints
SIMUL:=$(OUT)/s/simul
AUTOSIMUL:=$(OUT)/s/auto
FATTACKS:=$(OUT)/fastattacks
CATTACKS:=$(OUT)/chargedattacks
DUALTYPES:=$(OUT)/dualtypes
SPECIES:=$(OUT)/species
FORMS:=$(OUT)/forms
HETERO:=$(OUT)/hetero
TRELS:=$(OUT)/typerels
TIMETOFIRST:=$(OUT)/timetofirst
DUALCHARGED:=$(OUT)/dualcharged
COVERSETS:=$(OUT)/coversets
TEAMBUILDER:=$(OUT)/teambuilder
CALCULATOR:=$(OUT)/calculator
USERS:=$(OUT)/users
REGIONS:=$(OUT)/regions
REGIONAL:=$(OUT)/regional
WEATHER:=$(OUT)/weather
POPS:=$(OUT)/populations
DPEVSE:=$(OUT)/dpevse
PPTVST:=$(OUT)/pptvst
PAIRS:=$(OUT)/pairs
EPTVST:=$(OUT)/eptvst
ATKTYPES:=$(OUT)/attacktypes
SINDEX:=$(OUT)/sindex
FATTACKTEX:=$(OUT)/fastattacks.tex
CATTACKTEX:=$(OUT)/chargedattacks.tex
DUALTYPESTEX:=$(OUT)/dualtypes.tex
DPEVSEDAT:=$(DPEVSE).dat
PPTVSTDAT:=$(PPTVST).dat
EPTVSTDAT:=$(EPTVST).dat
DUALSUMSTEX:=$(OUT)/dualsummaries.tex
FUSERSTEX:=$(OUT)/fastusers.tex
CUSERSTEX:=$(OUT)/chargedusers.tex
ATKTYPESTEX:=$(OUT)/attacktypes.tex
CPMSTEX:=$(OUT)/cpms.tex
ELITETEX:=$(OUT)/elite.tex
SPECIESTEX:=$(OUT)/species.tex
FPAIRSTEX:=$(OUT)/fpairs.tex
CPAIRSTEX:=$(OUT)/cpairs.tex
MEGATEX:=$(OUT)/mega.tex
#DYNATEX:=$(OUT)/dynas.tex
SINDEXTEX:=$(SINDEX).tex
STATMAXTEX:=$(OUT)/statmax.tex
GIGANTATEX:=$(OUT)/gigantas.tex
WEATHERTEX:=$(WEATHER).tex
POPSTEX:=$(POPS).tex
REGIONSTEX:=$(REGIONS).tex
REGIONALTEX:=$(REGIONAL).tex
CP1500A:=$(OUT)/cp1500a.tex
CP2500A:=$(OUT)/cp2500a.tex
CP1500G:=$(OUT)/cp1500g.tex
CP2500G:=$(OUT)/cp2500g.tex
SUPERCOUNTERTEX:=$(OUT)/supercounter.tex
NOSECONDCHARGEDTEX:=$(OUT)/nosecondcharged.tex
HETEROCOSTTEX:=$(OUT)/heterocost.tex
HETEROTYPETEX:=$(OUT)/heterotype.tex
TRELSTEX:=$(OUT)/typerels.tex
EVENENERGYTEX:=$(OUT)/evenenergy.tex
DUALCHARGEDTEX:=$(DUALCHARGED).tex
TIMETOFIRSTTEX:=$(TIMETOFIRST).tex
TIMETOFIRSTFULLTEX:=$(OUT)/timetofirstfull.tex
DAMAGECYCLETEX:=$(OUT)/damagecycle.tex
DOUBLEDOUBLESTEX:=$(OUT)/doubledoubles.tex
TRIPLERESISTTEX:=$(OUT)/tripleresist.tex
EVOLNONEWATTACKS:=$(OUT)/evolnonewattacks.tex
CHARGEDBYENERGYTEX:=$(OUT)/chargedbyenergy.tex
ASETSTEX:=$(OUT)/asets.tex
XPTEX:=$(OUT)/xp.tex
LEVELS40TEX:=$(OUT)/xp40.tex
LEVELS80TEX:=$(OUT)/xp80.tex
LEVELUPGRADESTEX:=$(OUT)/xpupgrades.tex
BREAKPOINTSDAT:=$(BREAKPOINTS).tex
UNPOPTEX:=$(OUT)/unpop.tex
BULK1500TEX:=$(OUT)/bulk1500.tex
BULK2500TEX:=$(OUT)/bulk2500.tex
SIMULOUT:=$(OUT)/simulout.lst
SIMUL3OUT:=$(OUT)/simul3out.lst
SCARDEXAMPLETEX:=$(OUT)/scardexample.tex
OPTSTATSDAT:=$(OUT)/optstats.r
OPTSTATSR:=optstats.R
OPTSTATSPDF:=$(OUT)/optstats.pdf
OPTSTATSRADARDAT:=$(OUT)/optstatsradar.r
OPTSTATSRADARR:=optstatsradar.R
OPTSTATSRADARPDF:=$(OUT)/optstatsradar.pdf
GENTEX:=$(DUALTYPESTEX) $(SPECIESTEX) $(MEGATEX) $(CP1500A) $(CP2500A) \
	$(FATTACKTEX) $(CATTACKTEX) $(HETEROCOSTTEX) $(HETEROTYPETEX) $(TRELSTEX) \
	$(GIGANTATEX) $(ATKTYPESTEX) $(WEATHERTEX) $(REGIONSTEX) $(REGIONALTEX) \
	$(CUSERSTEX) $(FUSERSTEX) $(DUALSUMSTEX) $(POPSTEX) $(CPAIRSTEX) \
	$(SINDEXTEX) $(DPEVSEDAT) $(PPTVSTDAT) $(EPTVSTDAT) \
	$(BREAKPOINTSDAT) $(FPAIRSTEX) $(CP1500G) $(CP2500G) \
	$(TIMETOFIRSTTEX) $(DUALCHARGEDTEX) $(SIMULOUT) $(SIMUL3OUT) \
	$(BULK1500TEX) $(BULK2500TEX) $(EVENENERGYTEX) $(CPMSTEX) $(ELITETEX) \
	$(NOSECONDCHARGEDTEX) $(DOUBLEDOUBLESTEX) $(TRIPLERESISTTEX) $(DAMAGECYCLETEX) \
	$(UNPOPTEX) $(ASETSTEX) $(SUPERCOUNTERTEX) $(CHARGEDBYENERGYTEX) $(XPTEX) \
	$(SCARDEXAMPLETEX) $(EVOLNONEWATTACKS) $(LEVELS40TEX) $(LEVELS80TEX) \
	$(LEVELUPGRADESTEX)
GRAPHS:=$(addprefix $(OUT)/, $(addsuffix .png, $(CIRCO) $(DOT))) \
	octave/greninja.png
SIMULSRC:=$(addprefix s/, main.cpp cache.h simul.h bottom.h damage.h in.h ko.h sift.h memo.h simul.h top.h)
AUTOSIMULSRC:=$(addprefix s/, auto.cpp cache.h simul.h bottom.h damage.h in.h ko.h sift.h memo.h simul.h top.h)
WEBSRC:=$(wildcard w/*.cpp w/*.h)

preview: $(PAPERBACK).pdf
	$(PDFVIEW) $<

all: $(TEAMBUILDER) $(OPTSTATSPDF) $(OPTSTATSRADARPDF) $(CALCULATOR) $(PDF) $(SIMUL) $(BIGLISTS) \
	webpage testdb testsimul $(COVERSETS) test

CSS:=w/web.css
WEBCSS:=$(OUT)/$(CSS)
WEBOUT:=$(OUT)/w
WEBBIN:=$(OUT)/webbin
WEBINDEX:=$(WEBOUT)/index.html
webpage: $(WEBINDEX)

$(OPTSTATSRADARPDF): $(OPTSTATSRADARR) $(OPTSTATSRADARDAT)
	@mkdir -p $(@D)
	Rscript $<

$(OPTSTATSRADARDAT): $(OUT)/optstatsradar
	@mkdir -p $(@D)
	$< > $@

$(OPTSTATSPDF): $(OPTSTATSR) $(OPTSTATSDAT)
	@mkdir -p $(@D)
	Rscript $<

$(OPTSTATSDAT): $(OUT)/optstats
	@mkdir -p $(@D)
	$< > $@

$(WEBINDEX): $(WEBBIN) $(WEBCSS)
	@mkdir -p $(WEBOUT)
	#ln -sf $(shell pwd)/images/highres $(WEBOUT)/images
	$< $(WEBOUT)

$(WEBCSS): $(CSS)
	@mkdir -p $(WEBOUT)
	ln -sf $(shell pwd)/$< $@

$(WEBBIN): $(WEBSRC) pgotypes.h
	@mkdir -p $(@D)
	g++ -std=c++23 -g -o $@ -Wall -W -fno-var-tracking-assignments -O2 $(filter %.cpp,$^) -lm

$(PAPERBACK).pdf: $(TEX) $(MAKEFILE) $(GRAPHS) $(GENTEX) $(PAPERBACK).toc
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(MAKEFILE) $(GRAPHS) $(GENTEX)
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

list%.pdf: list%.tex $(OUT)/stat%.tex
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\input{$(<F)}' $(TEXOPTS)
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\input{$(<F)}' $(TEXOPTS)

listTTF.pdf: listTTF.tex $(TIMETOFIRSTFULLTEX)
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\input{$(<F)}' $(TEXOPTS)
	$(TEXCC) --shell-escape -jobname=$(basename $(@F)) '\input{$(<F)}' $(TEXOPTS)

$(OUT)/dot/%.png: dot/%
	@mkdir -p $(@D)
	dot -Tpng $< > $@

$(OUT)/circo/%.png: circo/%
	@mkdir -p $(@D)
	circo -Tpng $< > $@

$(OUT)/statGL.tex: $(OUT)/stat
	@mkdir -p $(@D)
	$(<) 1500 > $@

$(OUT)/statUL.tex: $(OUT)/stat
	@mkdir -p $(@D)
	$(<) 2500 > $@

$(LEVELUPGRADESTEX): $(OUT)/xp
	@mkdir -p $(@D)
	$(<) upgrades > $@

$(LEVELS40TEX): $(OUT)/xp
	@mkdir -p $(@D)
	$(<) 2 40 > $@

$(LEVELS80TEX): $(OUT)/xp
	@mkdir -p $(@D)
	$(<) 41 80 > $@

$(OUT)/statML.tex: $(OUT)/stat
	@mkdir -p $(@D)
	$(<) > $@

$(OUT)/statTTF.tex: $(OUT)/stat
	@mkdir -p $(@D)
	$(<) > $@

$(OUT)/dualtypes.tex: $(OUT)/dualtypes
	@mkdir -p $(@D)
	$(<) > $@

$(OUT)/dualsummaries.tex: $(OUT)/dualtypes
	@mkdir -p $(@D)
	$(<) summary > $@

$(BULK1500TEX): $(OUT)/bounded $(MAKEFILE)
	@mkdir -p $(@D)
	$(<) b 1500 127.1 > $@

$(BULK2500TEX): $(OUT)/bounded $(MAKEFILE)
	@mkdir -p $(@D)
	$(<) b 2500 163.0 > $@

$(OUT)/cp1500a.tex: $(OUT)/bounded $(MAKEFILE)
	@mkdir -p $(@D)
	$(<) a 1500 128.75 > $@

$(OUT)/cp1500g.tex: $(OUT)/bounded $(MAKEFILE)
	@mkdir -p $(@D)
	$(<) g 1500 124.83 > $@

$(OUT)/cp2500a.tex: $(OUT)/bounded $(MAKEFILE)
	@mkdir -p $(@D)
	$(<) a 2500 161.96 > $@

$(OUT)/cp2500g.tex: $(OUT)/bounded $(MAKEFILE)
	@mkdir -p $(@D)
	$(<) g 2500 156.85 > $@

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

$(HETEROTYPETEX): $(HETERO)
	@mkdir -p $(@D)
	$(<) type > $@

$(HETEROCOSTTEX): $(HETERO)
	@mkdir -p $(@D)
	$(<) cost > $@

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

$(DUALCHARGEDTEX): $(DUALCHARGED)
	@mkdir -p $(@D)
	$(<) d > $@

$(BREAKPOINTSDAT): $(basename $(BREAKPOINTSDAT))
	@mkdir -p $(@D)
	$< clodsire max "poison sting" "megahorn/earthquake" \
  	sawk 15-14-14@99 "low kick" "focus blast/low sweep" > $@

$(DAMAGECYCLETEX): $(basename $(TIMETOFIRSTTEX))
	@mkdir -p $(@D)
	$(<) extrema damage > $@

$(TIMETOFIRSTFULLTEX): $(basename $(TIMETOFIRSTTEX))
	@mkdir -p $(@D)
	$(<) > $@

$(TIMETOFIRSTTEX): $(basename $(TIMETOFIRSTTEX))
	@mkdir -p $(@D)
	$(<) extrema > $@

$(OUT)/%.dat: $(OUT)/%
	@mkdir -p $(@D)
	$(<) > $@

$(SIMULOUT): $(SIMUL)
	@mkdir -p $(@D)
	$(<) clodsire 15-15-15@70 "poison sting" "megahorn" \
				sawk 15-14-14@50 "low kick" "focus blast" > $@

$(SIMUL3OUT): $(SIMUL)
	@mkdir -p $(@D)
	$(<) clodsire opt1500 "poison sting" megahorn \
				talonflame opt1500 incinerate "brave bird" \
				quagsire opt1500 "water gun" earthquake \
				milotic opt1500 waterfall surf > $@

$(AUTOSIMUL): $(AUTOSIMULSRC) pgotypes.h Makefile
	@mkdir -p $(@D)
	g++ -std=c++23 -funroll-all-loops -g -march=native -mtune=native -o $@ -Wall -W -fno-var-tracking-assignments -O3 $(<) -lm -lmurmurhash

$(SIMUL): $(SIMULSRC) pgotypes.h Makefile
	@mkdir -p $(@D)
	g++ -std=c++23 -funroll-all-loops -g -march=native -mtune=native -o $@ -Wall -W -fno-var-tracking-assignments -O3 $(<) -lm -lmurmurhash

$(OUT)/%: %.cpp pgotypes.h Makefile
	@mkdir -p $(@D)
	g++ -std=c++23 -g -o $@ -Wall -W -O2 -fno-var-tracking-assignments $(<F) -lm

test: $(TEAMBUILDER) testsimul testdb
	$< metagross -u 0-0-0@1 'zen headbutt' psychic
	$< metagross -g 0-4-15@29 'zen headbutt' psychic
	$< 'shadow metagross' 15-15-15@99 'zen headbutt' 'psychic/meteor mash'

testdb: $(TESTDB)
	$(<)

testsimul: $(SIMUL)
	time $(<) clodsire opt1500 "poison sting" "megahorn" sawk opt1500 "low kick" "focus blast"
	time $(<) sawk opt1500 "low kick" "focus blast" clodsire opt1500 "poison sting" "megahorn"

testsimullong: $(SIMUL)
	time $(<) clodsire opt1500 "poison sting" "earthquake" sawk opt1500 "low kick" "low sweep"
	time $(<) clodsire max "poison sting" "megahorn/earthquake" sawk max "low kick" "focus blast/low sweep"
	time $(<) clodsire opt1500 "poison sting" "megahorn" sawk opt1500 "low kick" "focus blast" \
						feebas opt1500 tackle "mirror coat" milotic opt1500 waterfall surf
	time $(<) clodsire max "poison sting" "megahorn" sawk max "low kick" "focus blast" \
						feebas max tackle "mirror coat" milotic max waterfall surf
	time $(<) clodsire opt1500 "poison sting" "earthquake/megahorn" \
						talonflame opt1500 incinerate "brave bird/fire blast" \
						'shadow jumpluff' opt1500 "bullet seed" "acrobatics/solar beam" \
						dusknoir opt1500 astonish "shadow ball/dynamic punch"
	time $(<) clodsire opt750 "poison sting" "earthquake/megahorn" \
						talonflame opt750 incinerate "brave bird/fire blast" \
						quagsire opt750 "water gun" "earthquake/sludge bomb" \
						jumpluff opt750 "bullet seed" "acrobatics/solar beam" \
						dusknoir opt750 astonish "shadow ball/dynamic punch" \
						gallade opt750 confusion "synchronoise/leaf blade"

clean:
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof $(PAPERBACK).listing $(PAPERBACK).pdf
	@rm -vrf $(BIGLISTS)
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
	@rm -vrf svg-inkscape
	@rm -vrf _minted
	@rm -vf tags
