all:
	make pdf
	make bib
	make pdf
	make pdf

pdf:
	xelatex userManual.tex

bib:
	bibtex userManual

clean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.out
	rm -f *.toc
