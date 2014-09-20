TARGET=heresy
TEXFILES = $(wildcard *.tex)

all: $(TARGET).pdf

$(TARGET).aux: $(TEXFILES)
	latex $(TARGET).tex

$(TARGET).bbl: $(TARGET).aux $(TARGET).bib
	bibtex $(TARGET)

$(TARGET).dvi: $(TEXFILES) $(TARGET).bbl
	latex $(TARGET).tex
	latex $(TARGET).tex

$(TARGET).ps: $(TARGET).dvi
	dvips $< -Ppdf -Pcmz -Pamz -t letter -D 600 -G0 -o $@

$(TARGET).pdf: $(TARGET).ps
	ps2pdf $<

spell::
	ispell *.tex

final: $(TARGET).pdf
	exiftool -all:all= $(TARGET).pdf
	qpdf --linearize $(TARGET).pdf $(TARGET)-final.pdf

FIGURES = throughput latency

graphs: $(addprefix figures/, $(addsuffix .eps, $(FIGURES)))

figures/%.eps: plots/%.plots data/%/summary.data
	gnuplot $< > $@
