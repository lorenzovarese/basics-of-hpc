all: recur_seq recur_omp

recur_seq: recur_seq.c
	g++-13 -O3 $^ -o $@

recur_omp: recur_omp.c
	g++-13 -O3 -fopenmp $^ -o $@ -lm

clean:
	rm -rf recur_seq recur_omp

