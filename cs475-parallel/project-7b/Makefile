CC = mpic++ -lm -g

mpi-correlation: mpi-correlation.cpp
	$(CC) -o mpi-correlation mpi-correlation.cpp

mpi-correlation-csv: mpi-correlation.cpp
	$(CC) -o mpi-correlation-csv mpi-correlation.cpp -DCSV -DDEBUG=0

bench-local: mpi-correlation-csv
	echo numprocs,numelements,perf > results.csv
	for p in {2..12..2}; do \
   	mpiexec --use-hwthread-cpus --mca opal_warn_on_missing_libcuda 0 -mca btl self,tcp -np $$p ./mpi-correlation-csv; \
	done >> results.csv

bench: mpi-correlation-csv
	echo numprocs,numelements,perf > results.csv
	for p in {2..12..2}; do \
   	mpiexec -mca btl self,tcp -np $$p ./mpi-correlation-csv; \
	done >> results.csv

report: report.md
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
