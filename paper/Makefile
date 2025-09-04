# Simple Makefile for building CBT paper

.PHONY: all clean

all: cbt.pdf

cbt.pdf: cbt.tex
	pdflatex -interaction=nonstopmode cbt.tex
	-bibtex cbt
	pdflatex -interaction=nonstopmode cbt.tex
	pdflatex -interaction=nonstopmode cbt.tex

clean:
	rm -f *.aux *.log *.out *.bbl *.blg *.toc *.lof *.lot

distclean: clean
	rm -f cbt.pdf