CC = g++ -g -lm -fopenmp -lOpenCL

GLOBAL_WORKS = $(shell ruby -e 'puts (10..23).map {|p| 2**p }')
LOCAL_WORKS = $(shell ruby -e 'puts (3..9).map {|p| 2**p }')

first: first.cpp
	$(CC) -o first first.cpp

printinfo: printinfo.cpp
	$(CC) -o printinfo printinfo.cpp

bench results.csv: montecarlo.cu Makefile
	echo 'trials,blocksize,performance,probability' > results.csv
	for t in $(TRIALS); do \
		for bs in $(BLOCKS); do \
			$(CUDA_NVCC) -o montecarlo-csv montecarlo.cu -DNUMTRIALS=$$t -DBLOCKSIZE=$$bs -DCSV; \
			./montecarlo-csv;\
		done;  \
	done >> results.csv;

pivot results-blocksize.csv: results.csv
	mlr --csv cut -x -f probability then reshape -s trials,performance results.csv > results-blocksize.csv
	mlr --csv cut -x -f probability then reshape -s blocksize,performance results.csv > results-trials.csv

report: report.md results.csv results-blocksize.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
