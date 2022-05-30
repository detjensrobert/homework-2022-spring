CC = g++ -g -lm -fopenmp -lOpenCL

GLOBAL_WORKS = $(shell ruby -e 'puts (10..23).map {|p| 2**p }')
LOCAL_WORKS = $(shell ruby -e 'puts (3..8).map {|p| 2**p }')

runner: runner.cpp
	$(CC) -o runner runner.cpp

printinfo: printinfo.cpp
	$(CC) -o printinfo printinfo.cpp

results-mult.csv: runner.cpp array-mult.cl
	echo 'globalsize,localsize,workgroups,performance' > results-mult.csv
	for t in $(GLOBAL_WORKS); do \
		for bs in $(LOCAL_WORKS); do \
			$(CC) -o runner-csv runner.cpp -DCSV -DLOCAL_SIZE=$$bs -DNUM_ELEMENTS=$$t -DCL_FILE_NAME='"array-mult.cl"' ; \
			./runner-csv;\
		done;  \
	done >> results-mult.csv;

results-multadd.csv: runner.cpp array-mult.cl
	echo 'globalsize,localsize,workgroups,performance' > results-multadd.csv
	for t in $(GLOBAL_WORKS); do \
		for bs in $(LOCAL_WORKS); do \
			$(CC) -o runner-csv runner.cpp -DCSV -DLOCAL_SIZE=$$bs -DNUM_ELEMENTS=$$t -DCL_FILE_NAME='"array-mult-sum.cl"' ; \
			./runner-csv;\
		done;  \
	done >> results-multadd.csv;

pivot results-mult-local.csv: results-mult.csv results-multadd.csv
	mlr --csv cut -x -f workgroups then reshape -s globalsize,performance results-mult.csv > results-mult-local.csv
	mlr --csv cut -x -f workgroups then reshape -s localsize,performance  results-mult.csv > results-mult-global.csv

	mlr --csv cut -x -f workgroups then reshape -s globalsize,performance results-multadd.csv > results-multadd-local.csv
	mlr --csv cut -x -f workgroups then reshape -s localsize,performance  results-multadd.csv > results-multadd-global.csv

report: report.md results-mult.csv results-multadd.csv results-mult-local.csv
	pandoc -s report.md -o report.pdf -V geometry:margin=1in --highlight=tango -F pantable

.PHONY: clean
clean:
	git clean -dfX
