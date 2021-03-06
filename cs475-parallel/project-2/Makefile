BIN_NAME = integration

THREADS = 1 2 4 8 12 16 20
# THREADS = 1 2 4 8 12 16 20 24 32
SLICES = 50 100 500 1000 2000 4000 8000
# SLICES = 1000 2000 4000 8000 16000

$(BIN_NAME): $(BIN_NAME).cpp
	g++ -O3 -o $(BIN_NAME) $(BIN_NAME).cpp -lm -fopenmp

$(BIN_NAME)-%t: $(BIN_NAME).cpp
	g++ -O3 -o $@ $(BIN_NAME).cpp -lm -fopenmp -DNUMT=$*

bin: $(BIN_NAME).cpp
	[ -d bin ] || mkdir bin

	parallel g++ -O3 -o bin/$(BIN_NAME)-{1}th-{2}n $(BIN_NAME).cpp -lm -fopenmp -DNUMT={1} -DNUMSLICES={2} -DCSV -DQUIET \
	  ::: $(THREADS) ::: $(SLICES)

bench results.csv: bin
	# remove previous resuts
	echo 'threads,slices,volume,performance' > results.csv

	# run one trial at a time
	# hyperfine -N --export-csv results.hyp.csv bin/*
	for f in bin/*; do printf "$$f ..."; ./$$f >> results.csv; echo -n .; printf "\r"; done;

	# sort the data so the graphs work
	mlr -I --csv sort -n threads,slices results.csv

	# pivot data
	mlr --csv cut -x -f volume then reshape -s threads,performance results.csv > results-slices.csv
	mlr --csv cut -x -f volume then reshape -s slices,performance results.csv > results-threads.csv

report: report.md results.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
