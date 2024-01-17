all: hist_seq hist_omp hist_omp_benchmark

hist_seq: hist_seq.cpp
	g++-13 -O3 $^ -o $@

hist_omp: hist_omp.cpp
	g++-13 -O3 -fopenmp $^ -o $@

hist_omp_benchmark: hist_omp_benchmark.cpp
	g++-13 -O3 -fopenmp $^ -o $@

clean:
	rm -rf hist_seq hist_omp hist_omp_benchmark

