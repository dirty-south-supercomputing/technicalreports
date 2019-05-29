.DELETE_ON_ERROR:
.PHONY: all reports preview clean
.DEFAULT_GOAL:=preview

UNANDFS:=μnandfs
LATEX:=xelatex
BIBTEX:=biber
VIEW:=evince

all: reports

reports: $(UNANDFS).pdf

preview: reports
	$(VIEW) $(UNANDFS).pdf

RESOURCES:=

$(UNANDFS).pdf: $(UNANDFS).tex $(UNANDFS).bib $(RESOURCES)
	$(LATEX) $<
	$(BIBTEX) $(basename $@)
	$(LATEX) $<
	$(LATEX) $<

clean:
	rm -rf $(wildcard *.aux) $(wildcard *.bbl) $(wildcard *.bcf) $(wildcard *.blg)
	rm -rf $(wildcard *.log) $(wildcard *.out) $(wildcard *.pdf) $(wildcard *.xml)
