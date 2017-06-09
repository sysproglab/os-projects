all:
	make pdf
	make bib
	make pdf
	make pdf

pdf:
	xelatex tests.tex

bib:
	bibtex tests

clean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.out
	rm -f *.toc
