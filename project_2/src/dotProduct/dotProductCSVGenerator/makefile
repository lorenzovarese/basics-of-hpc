all: dotProductSerial dotProductReduction dotProductCritical

dotProductSerial: dotProductSerial.cpp walltime.h
	g++-13 -O3 -fopenmp $< -o $@

dotProductReduction: dotProductReduction.cpp walltime.h
	g++-13 -O3 -fopenmp $< -o $@

dotProductCritical: dotProductCritical.cpp walltime.h
	g++-13 -O3 -fopenmp $< -o $@

clean:
	rm -rf dotProductSerial dotProductReduction dotProductCritical
