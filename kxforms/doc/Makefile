MAINFILE       := kxforms

all: $(MAINFILE).pdf

$(MAINFILE).pdf:
	- rm $(MAINFILE).aux $(MAINFILE).ind
	- pdflatex $(MAINFILE)
	- pdflatex $(MAINFILE)

clean:
	- find . | egrep ".*((\.(aux|idx|ind|ilg|log|blg|bbl|toc|lof|lot|dvi|tlf|tlt|out))|~)$$" | xargs rm
