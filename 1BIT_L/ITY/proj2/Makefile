NAME=proj2

$(NAME).pdf: $(NAME).ps
	ps2pdf $(NAME).ps

$(NAME).ps: $(NAME).dvi
	dvips -t a4 $(NAME).dvi

$(NAME).dvi: $(NAME).tex
	latex $(NAME).tex
	latex $(NAME).tex

.PHONY:
	clean pack
clean:
	-rm $(NAME).dvi $(NAME).aux $(NAME).toc $(NAME).log $(NAME).out

pack:
	-tar -cvzf xkocic01-fit.tar.gz $(NAME).tex Makefile
