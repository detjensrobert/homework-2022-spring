BIN_NAME = grain-sim

THREADS = 1 2 4 8 12 16 20 24 32
NODES = 5 10 50 100 500 1000 2000 4000 8000 16000

$(BIN_NAME): $(BIN_NAME).cpp
	g++ -O3 -o $(BIN_NAME) $(BIN_NAME).cpp -lm -fopenmp

$(BIN_NAME)-csv: $(BIN_NAME).cpp
	g++ -O3 -o $(BIN_NAME)-csv $(BIN_NAME).cpp -lm -fopenmp -DCSV

bench results.csv: $(BIN_NAME)-csv
	./$(BIN_NAME)-csv > results.csv

graphs: results.csv
	[ -d graphs ] || mkdir graphs

report: report.md results.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
