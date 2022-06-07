CC = mpic++

mpi-correlation: mpi-correlation.cpp
	$(CC) -o mpi-correlation mpi-correlation.cpp



report: report.md results.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
