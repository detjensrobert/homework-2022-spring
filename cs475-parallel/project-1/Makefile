BIN_NAME = montecarlo

THREADS = 1 2 4 8 12 16 20 24 32
TRIALS = 1 10 100 1000 10000 100000 1000000

$(BIN_NAME): $(BIN_NAME).cpp
	g++ -o $(BIN_NAME) $(BIN_NAME).cpp -lm -fopenmp

$(BIN_NAME)-%t: $(BIN_NAME).cpp
	g++ -o $@ $(BIN_NAME).cpp -lm -fopenmp -DNUMT=$* -DQUIET

bin/$(BIN_NAME)-1th-1tr: $(BIN_NAME).cpp
	[ -d bin ] || mkdir bin

	parallel g++ -o bin/$(BIN_NAME)-{1}th-{2}tr $(BIN_NAME).cpp -lm -fopenmp -DNUMT={1} -DNUMTRIALS={2} -DCSV -DQUIET \
	  ::: $(THREADS) ::: $(TRIALS)

bench results.csv: bin/$(BIN_NAME)-1th-1tr
	# remove previous resuts
	echo 'threads,trials,probability,performance' > results.csv

	# run one trial at a time
	# hyperfine -N --export-csv results.hyp.csv bin/*
	for f in bin/*; do ./$$f >> results.csv; echo -n .; done; echo

	# sort the data so the graphs work
	mlr -I --csv sort -n threads,trials results.csv

report: report.md results.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pandoc-include -F pantable

.PHONY: clean
clean:
	git clean -dfX
