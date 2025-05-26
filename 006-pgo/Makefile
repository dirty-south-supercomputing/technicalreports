.DELETE_ON_ERROR:
.PHONY: all preview clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=$(addsuffix .pdf, $(PAPERBACK))
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo)
PDFVIEW:=evince

preview: all
	$(PDFVIEW) $(PAPERBACK).pdf

all: $(OUT)

$(PAPERBACK).pdf: $(TEX) $(IMAGES) $(PAPERBACK).toc
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(IMAGES)
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

clean:
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
