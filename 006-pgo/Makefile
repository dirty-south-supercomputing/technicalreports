.DELETE_ON_ERROR:
.PHONY: all preview clean
.DEFAULT_GOAL:=all

PAPERBACK:=pgo-pb
OUT:=out
TEXCC:=lualatex
TEXOPTS:=-shell-escape -halt-on-error
TEX:=$(addsuffix .tex, pgo trainer types attacks species pokémon stats damage optimize)
CIRCO:=$(addprefix circo/, $(addsuffix .dot, dragon nature phases rational steel))
DOT:=$(addprefix dot/, $(addsuffix .dot, death jumble))
GRAPHS:=$(addprefix $(OUT)/, $(addsuffix .png, $(CIRCO) $(DOT)))
PDFVIEW:=evince
PDF:=$(addsuffix .pdf, $(PAPERBACK))

preview: all
	$(PDFVIEW) $(PAPERBACK).pdf

all: $(PDF)

$(PAPERBACK).pdf: $(TEX) $(IMAGES) $(PAPERBACK).toc
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(PAPERBACK).toc: $(TEX) $(GRAPHS)
	$(TEXCC) -jobname=$(basename $(@F)) '\def\paperback{yes}\input{$(<F)}' $(TEXOPTS)

$(OUT)/dot/%.png: dot/%
	@mkdir -p $(@D)
	dot -Tpng $< > $@

$(OUT)/circo/%.png: circo/%
	@mkdir -p $(@D)
	circo -Tpng $< > $@

clean:
	@rm -vrf $(PAPERBACK).toc $(PAPERBACK).log $(PAPERBACK).aux $(PAPERBACK).lof
	@rm -vrf pdfa.xmpi
	@rm -vrf $(OUT)
