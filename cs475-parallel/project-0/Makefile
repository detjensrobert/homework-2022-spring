BIN_NAME = simple-openmp

$(BIN_NAME): $(BIN_NAME).cpp
	g++ -o $(BIN_NAME) $(BIN_NAME).cpp -fopenmp

$(BIN_NAME)-%t: $(BIN_NAME).cpp
	g++ -o $@ $(BIN_NAME).cpp -fopenmp -DNUMT=$* -DQUIET

out.%t: $(BIN_NAME)-%t
	./$(BIN_NAME)-$*t > $@

bench: $(BIN_NAME)-1t $(BIN_NAME)-4t
	hyperfine --export-markdown results.md ./$(BIN_NAME)-1t ./$(BIN_NAME)-4t

report: report.md out.1t out.4t
	pandoc -o report.pdf -s report.md -V geometry:margin=1in --highlight=tango -F pandoc-include -F pandoc-run-filter

.PHONY: clean
clean:
	rm -f $(shell cat .gitignore)
