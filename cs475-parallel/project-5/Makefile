CC = g++ -g -lm -fopenmp -O0

TRIALS = $(shell ruby -e 'puts (10..25).map {|p| 2**p }')
BLOCKS = 8 32 128

# this only works on rabbit / the HPC cluster!
CUDA_PATH	=	/usr/local/apps/cuda/cuda-10.1
CUDA_BIN_PATH	=	$(CUDA_PATH)/bin
CUDA_NVCC	=	$(CUDA_BIN_PATH)/nvcc

montecarlo:	ensure_cuda montecarlo.cu
	$(CUDA_NVCC) -o montecarlo montecarlo.cu

montecarlo-csv:	ensure_cuda montecarlo.cu
	$(CUDA_NVCC) -o montecarlo-csv montecarlo.cu -DCSV

bench: montecarlo.cu Makefile
	echo 'trials,blocksize,performance,probability' > results.csv
	for t in $(TRIALS); do \
		for bs in $(BLOCKS); do \
			$(CUDA_NVCC) -o montecarlo-csv montecarlo.cu -DNUMTRIALS=$$t -DBLOCKSIZE=$$bs -DCSV; \
			./montecarlo-csv;\
		done;  \
	done >> results.csv;

bench-rabbit results.csv: Makefile
	ssh rabbit.engr.oregonstate.edu 'cd ~/repos/homework-2022-spring/cs475-parallel/project-5 && git pull -q && make bench'
	scp rabbit.engr.oregonstate.edu:repos/homework-2022-spring/cs475-parallel/project-5/results.csv results.csv

pivot results-blocksize.csv: results.csv
	mlr --csv cut -x -f probability then reshape -s trials,performance results.csv > results-blocksize.csv
	mlr --csv cut -x -f probability then reshape -s blocksize,performance results.csv > results-trials.csv

report: report.md results.csv results-blocksize.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX

ensure_cuda:
	test -f $(CUDA_NVCC)
