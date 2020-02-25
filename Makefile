.DELETE_ON_ERROR:
.PHONY: all reports clean
.DEFAULT_GOAL:=all

CANBUSBASE:=canbusre
CANBUSDIR:=001-$(CANBUSBASE)
µNANDFSBASE:=µnandfs
µNANDFSDIR:=002-$(µNANDFSBASE)
NOTCURSESBASE:=notcurses
NOTCURSESDIR:=004-$(NOTCURSESBASE)

# Common resources
RESOURCES:=common/dsscaw-purp-scaled.png dsscaw-hdr.pdf common/south.png

µNANDFSRESOURCES:=$(addsuffix .png,combined ExtendBlob ReadBlob \
  FSM Initialize FindBlob)
NOTCURSESRESOURCES:=$(wildcard $(NOTCURSESDIR)/*.png) $(wildcard $(NOTCURSESDIR)/*.jpg) \
	$(wildcard $(NOTCURSESDIR)/code/*.c)
DIRS:=$(CANBUSDIR) $(µNANDFSDIR) $(NOTCURSESDIR)

REPORTS:=$(CANBUSDIR)/$(CANBUSBASE)
REPORTS+=$(µNANDFSDIR)/$(µNANDFSBASE)
REPORTS+=$(NOTCURSESDIR)/$(NOTCURSESBASE)
REPORTS:=$(addsuffix .pdf,$(REPORTS))

LATEX:=xelatex
BIBTEX:=biber
VIEW:=evince

NCBINS:=$(addprefix $(NOTCURSESDIR)/, hilodirect hilostdio hol-formatter speech \
          tetrimino tetrimino-input)

all: reports

reports: $(REPORTS)

$(µNANDFSDIR)/$(µNANDFSBASE).pdf: $(addprefix $(µNANDFSDIR)/,$(µNANDFSBASE).tex $(µNANDFSBASE).bib $(µNANDFSRESOURCES)) $(RESOURCES)
	cd $(@D) && arara -v $(<F)

$(NOTCURSESDIR)/$(NOTCURSESBASE).pdf: $(addprefix $(NOTCURSESDIR)/,$(NOTCURSESBASE).tex $(NOTCURSESBASE).bib glossary.tex simpleloop.tex) $(NOTCURSESRESOURCES) $(RESOURCES) $(NCBINS)
	cd $(@D) && arara -v $(<F)

$(CANBUSDIR)/$(CANBUSBASE).pdf: $(CANBUSDIR)/$(CANBUSBASE).tex $(CANBUSDIR)/$(CANBUSBASE).bib $(RESOURCES)
	cd $(@D) && arara -v $(<F)

$(NOTCURSESDIR)/tetrimino-input: $(NOTCURSESDIR)/code/tetrimino-input.c $(addsuffix .h, $(addprefix $(NOTCURSESDIR)/code/tetrimino-, databox displayutf8 drawcircle inputmain switch))
	$(CC) $(CFLAGS) -o $@ $< -lnotcurses -lm

$(NOTCURSESDIR)/tetrimino: $(NOTCURSESDIR)/code/tetrimino.c $(addsuffix .h, $(addprefix $(NOTCURSESDIR)/code/tetrimino-, display draw main mouse switch))
	$(CC) $(CFLAGS) -o $@ $< -lnotcurses

$(NOTCURSESDIR)/%: $(NOTCURSESDIR)/code/%.c
	$(CC) $(CFLAGS) -o $@ $< -lnotcurses

clean:
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.aux) $(wildcard $(dir)/*.bbl))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.bcf) $(wildcard $(dir)/*.blg))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.glg) $(wildcard $(dir)/*.gls) $(wildcard $(dir)/*.glo) $(wildcard $(dir)/*.ist))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.log) $(wildcard $(dir)/*.out))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.pdf) $(wildcard $(dir)/*.xml))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.toc))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/_minted-*))
	@rm -vrf $(NCBINS)
