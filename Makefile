.DELETE_ON_ERROR:
.PHONY: all reports clean
.DEFAULT_GOAL:=all

CANBUSBASE:=canbusre
CANBUSDIR:=001-$(CANBUSBASE)
µNANDFSBASE:=µnandfs
µNANDFSDIR:=002-$(µNANDFSBASE)
NOTCURSESBASE:=notcurses
NOTCURSESDIR:=003-$(NOTCURSESBASE)

# Common resources
RESOURCES:=common/dsscaw-purp-scaled.png dsscaw-hdr.pdf

µNANDFSRESOURCES:=$(RESOURCES) $(addsuffix .png,combined ExtendBlob ReadBlob \
  FSM Initialize FindBlob south)
NOTCURSESRESOURCES:=$(RESOURCES) $(wildcard $(NOTCURSESDIR)/*.png) $(wildcard $(NOTCURSESDIR)/*.jpg)
DIRS:=$(CANBUSDIR) $(µNANDFSDIR) $(NOTCURSESDIR)

REPORTS:=$(CANBUSDIR)/$(CANBUSBASE)
REPORTS+=$(µNANDFSDIR)/$(µNANDFSBASE)
REPORTS:=$(NOTCURSESDIR)/$(NOTCURSESBASE)
REPORTS:=$(addsuffix .pdf,$(REPORTS))

LATEX:=xelatex
BIBTEX:=biber
VIEW:=evince

all: reports

reports: $(REPORTS)

$(µNANDFSDIR)/$(µNANDFSBASE).pdf: $(addprefix $(µNANDFSDIR)/,$(µNANDFSBASE).tex $(µNANDFSBASE).bib $(µNANDFSRESOURCES))
	cd $(@D) && arara -v $(<F)

$(NOTCURSESDIR)/$(NOTCURSESBASE).pdf: $(addprefix $(NOTCURSESDIR)/,$(NOTCURSESBASE).tex $(NOTCURSESBASE).bib) $(NOTCURSESRESOURCES)
	cd $(@D) && arara -v $(<F)

$(CANBUSDIR)/$(CANBUSBASE).pdf: $(CANBUSDIR)/$(CANBUSBASE).tex $(CANBUSDIR)/$(CANBUSBASE).bib
	cd $(@D) && arara -v $(<F)

clean:
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.aux) $(wildcard $(dir)/*.bbl))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.bcf) $(wildcard $(dir)/*.blg))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.glg) $(wildcard $(dir)/*.gls) $(wildcard $(dir)/*.glo) $(wildcard $(dir)/*.ist))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.log) $(wildcard $(dir)/*.out))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.pdf) $(wildcard $(dir)/*.xml))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.toc))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/_minted-*))
