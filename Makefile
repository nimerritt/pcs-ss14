FIGURES = throughput latency

all: graphs

graphs: $(addprefix figures/, $(addsuffix .eps, $(FIGURES)))

figures/%.eps: plots/%.plots data/%/summary.data
	gnuplot $< > $@
