BIN_NAME = montecarlo

THREADS = 1 2 4 8 # 12 16 20 24 32
TRIALS = 1 10 100 1000 10000 100000 1000000

$(BIN_NAME): $(BIN_NAME).cpp
	g++ -o $(BIN_NAME) $(BIN_NAME).cpp -lm -fopenmp

$(BIN_NAME)-%t: $(BIN_NAME).cpp
	g++ -o $@ $(BIN_NAME).cpp -lm -fopenmp -DNUMT=$* -DQUIET

bench results.csv: $(BIN_NAME).cpp
	[ -d bin ] || mkdir bin
	echo 'threads,trials,probability,performance' > results.csv 				# remove previous resuts

	# compile stuff in parallel
	parallel g++ -o bin/$(BIN_NAME)-{1}th-{2}tr $(BIN_NAME).cpp -lm -fopenmp -DNUMT={1} -DNUMTRIALS={2} -DCSV -DQUIET \
	  ::: $(THREADS) ::: $(TRIALS)

	# run one trial at a time
	hyperfine -N --export-csv results.hyp.csv bin/*
	for f in bin/*; do ./$$f >> results.csv; done


report: report.md results.csv
	pandoc -o report.pdf -s report.md -V geometry:margin=1in --highlight=tango -F pandoc-include -F pandoc-run-filter

.PHONY: clean
clean:
	rm -rf $(shell cat .gitignore)
