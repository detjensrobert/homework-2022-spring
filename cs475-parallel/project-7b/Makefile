CC = mpic++ -lm -g

mpi-correlation: mpi-correlation.cpp
	$(CC) -o mpi-correlation mpi-correlation.cpp

bench: mpi-correlation
	mpiexec --mca opal_warn_on_missing_libcuda 0 -mca btl self,tcp -np 4 ./mpi-correlation;

report: report.md results.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
