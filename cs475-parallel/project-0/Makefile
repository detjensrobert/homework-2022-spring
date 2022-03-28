simple-openmp: simple-openmp.cpp
	g++ -o simple-openmp simple-openmp.cpp -fopenmp

simple-openmp-%t: simple-openmp.cpp
	g++ -o $@ simple-openmp.cpp -fopenmp -DNUMT=$* -DQUIET

out.%t: simple-openmp-%t
	./simple-openmp-$*t > $@

bench: simple-openmp-1t simple-openmp-4t
	hyperfine --runs 1 --export-markdown results.md ./simple-openmp-1t ./simple-openmp-4t

report: report.md out.1t out.4t
	pandoc -o report.pdf -s report.md -V geometry:margin=1in --highlight=tango -F pandoc-include -F pandoc-run-filter

.PHONY: clean
clean:
	rm -f $(shell cat .gitignore)
