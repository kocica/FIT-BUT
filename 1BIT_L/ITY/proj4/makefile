NAME=proj4

$(NAME).pdf: $(NAME).ps
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

$(NAME).ps: $(NAME).dvi
	dvips -t a4 $(NAME).dvi

$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME)
	latex $(NAME).tex
	latex $(NAME).tex

.PHONY:
	clean pack
clean:
	-rm -f $(NAME).log $(NAME).dvi $(NAME).aux $(NAME).toc $(NAME).bbl $(NAME).blg $(NAME).out

pack:
	zip xkocic01-fit.zip $(NAME).tex $(NAME).bib czplain.bst makefile
