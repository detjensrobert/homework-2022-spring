CC = g++ -g -lm -fopenmp -O0

TRIALS = 1024 4096 16384 65536 262144 1048576 2097152 4194304
BLOCKS = 8 32 128

# this only works on rabbit / the HPC cluster!
CUDA_PATH	=	/usr/local/apps/cuda/cuda-10.1
CUDA_BIN_PATH	=	$(CUDA_PATH)/bin
CUDA_NVCC	=	$(CUDA_BIN_PATH)/nvcc

montecarlo:	ensure_cuda montecarlo.cu
	$(CUDA_NVCC) -o montecarlo montecarlo.cu

montecarlo-csv:	ensure_cuda montecarlo.cu
	$(CUDA_NVCC) -o montecarlo-csv montecarlo.cu -DCSV

bench results.csv: montecarlo-csv
	echo 'trials,blocksize,performance,probability' > results.csv
	for t in $(TRIALS); do \
		for bs in $(BLOCKS); do \
			$(CUDA_NVCC) -o montecarlo-csv montecarlo.cu -DNUMTRIALS=$$t -DBLOCKSIZE=$$bs -DCSV; \
			./montecarlo-csv 2>> results.csv; \
		done; \
	done

report: report.md results.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX

ensure_cuda:
	test -f $(CUDA_NVCC)

sample-arraymult:	ensure_cuda sample-arraymult.cu
	$(CUDA_NVCC) -o sample-arraymult sample-arraymult.cu
