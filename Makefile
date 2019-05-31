.DELETE_ON_ERROR:
.PHONY: all reports clean
.DEFAULT_GOAL:=all

CANBUSBASE:=canbusre
CANBUSDIR:=001-$(CANBUSBASE)
µNANDFSBASE:=µnandfs
µNANDFSDIR:=002-$(µNANDFSBASE)
DIRS:=$(CANBUSDIR) $(µNANDFSDIR)

REPORTS:=$(CANBUSDIR)/$(CANBUSBASE)
REPORTS+=$(µNANDFSDIR)/$(µNANDFSBASE)
REPORTS:=$(addsuffix .pdf,$(REPORTS))

LATEX:=xelatex
BIBTEX:=biber
VIEW:=evince

all: reports

reports: $(REPORTS)

$(µNANDFSDIR)/$(µNANDFSBASE).pdf: $(µNANDFSDIR)/$(µNANDFSBASE).tex $(µNANDFSDIR)/$(µNANDFSBASE).bib
	$(LATEX) -output-directory $(@D) $<
	$(BIBTEX) $(basename $@)
	$(LATEX) -output-directory $(@D)  $<
	$(LATEX) -output-directory $(@D)  $<

$(CANBUSDIR)/$(CANBUSBASE).pdf: $(CANBUSDIR)/$(CANBUSBASE).tex $(CANBUSDIR)/$(CANBUSBASE).bib
	$(LATEX) -output-directory $(@D) $<
	$(BIBTEX) $(basename $@)
	$(LATEX) -output-directory $(@D)  $<
	$(LATEX) -output-directory $(@D)  $<

clean:
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.aux) $(wildcard $(dir)/*.bbl))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.bcf) $(wildcard $(dir)/*.blg))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.log) $(wildcard $(dir)/*.out))
	@rm -vrf $(foreach dir,$(DIRS),$(wildcard $(dir)/*.pdf) $(wildcard $(dir)/*.xml))
