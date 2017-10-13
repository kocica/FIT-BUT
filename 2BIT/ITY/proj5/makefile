# makefile
# ITY proj5
# Filip Kocica xkocic01@fit.vutbr.cz
# 15/4/2017

NAME=proj5

all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

clean:
	rm -f $(NAME).aux $(NAME).log $(NAME).dvi $(NAME).ps $(NAME).out $(NAME).pdf

pack:
	zip xkocic01-fit.zip $(NAME).tex makefile PPRfyma2.sty donald_e_knuth.eps
