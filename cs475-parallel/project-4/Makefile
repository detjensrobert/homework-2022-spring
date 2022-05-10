CC = g++ -g -lm -fopenmp -O0

THREADS = 1 2 4 8 12 16 20 24 32
ARRAYSIZE = $(shell ruby -e 'puts (10..24).map {|p| 2**p }')

simd-test: simd-test.cpp
	$(CC) -o simd-test simd-test.cpp

simd-test-csv: simd-test.cpp
	$(CC) -o simd-test-csv simd-test.cpp -DCSV

simd-test-a%: simd-test.cpp
	$(CC) -o $@ simd-test.cpp -DARRAYSIZE=$* -DCSV

# if not on a centos system, this needs to run in a centos container
docker: Dockerfile simd-test.cpp
	docker build -q --tag project-4 .
	docker run --rm -it project-4 sh -c 'make simd-test && ./simd-test'

docker-bench results.csv: Dockerfile simd-test.cpp
	docker build -q --tag project-4 .
	echo 'array-size,mult-nosimd,mult-simd,mult-speedup,reduce-nosimd,reduce-simd,reduce-speedup' > results.csv
	for as in $(ARRAYSIZE); do \
		docker run --rm -it project-4 sh -c "make --silent simd-test-a$$as && ./simd-test-a$$as" >> results.csv; \
	done

flip-bench results-flip.csv: simd-test.cpp
	ssh flip 'cd ~/repos/homework-2022-spring/cs475-parallel/project-4 && git pull -q && make bench'
	scp flip:repos/homework-2022-spring/cs475-parallel/project-4/results.csv results-flip.csv

bench: simd-test.cpp
	echo 'array-size,mult-nosimd,mult-simd,mult-speedup,reduce-nosimd,reduce-simd,reduce-speedup' > results.csv
	for as in $(ARRAYSIZE); do \
		make --silent simd-test-a$$as && ./simd-test-a$$as >> results.csv; \
	done

report: report.md results.csv results-flip.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
