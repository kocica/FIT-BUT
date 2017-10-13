NAME=proj3

$(NAME).pdf: $(NAME).ps
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

$(NAME).ps: $(NAME).dvi
	dvips -t a4 $(NAME).dvi

$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex
	latex $(NAME).tex

.PHONY:
	clean pack
clean:
	-rm -f $(NAME).dvi $(NAME).aux $(NAME).toc $(NAME).log $(NAME).out

pack:
	zip xkocic01-fit.zip $(NAME).tex makefile algorithm2e.sty
