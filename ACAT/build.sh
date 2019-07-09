#!/bin/sh

pdflatex JPCSLaTeXGuidelines.tex
bibtex JPCSLaTeXGuidelines.aux
pdflatex JPCSLaTeXGuidelines.tex
