CC = g++ -g -lm -fopenmp -O0

TRIALS = 1024 4096 16384 65536 262144 1048576 2097152 4194304
BLOCKS = 8 32 128

# this only works on rabbit / the HPC cluster!
CUDA_PATH	=	/usr/local/apps/cuda/cuda-10.1
CUDA_BIN_PATH	=	$(CUDA_PATH)/bin
CUDA_NVCC	=	$(CUDA_BIN_PATH)/nvcc

ensure_cuda:
	@ test -f $(CUDA_NVCC) || (echo 'ERR: this system does not have CUDA installed!' 1>&2 ; exit 1)

sample-arraymult:	ensure_cuda sample-arraymult.cu
	$(CUDA_NVCC) -o sample-arraymult sample-arraymult.cu

montecarlo:	ensure_cuda montecarlo.cu
	$(CUDA_NVCC) -o montecarlo montecarlo.cu

bench results.csv: montecarlo
	touch results.csv

report: report.md results.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
