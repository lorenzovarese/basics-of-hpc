all: mandel_parallel

mandel_parallel: 
	g++-13 -O3 -fopenmp mandel_parallel.c -o $@ -I/opt/homebrew/Cellar/libpng/1.6.40/include -L/opt/homebrew/Cellar/libpng/1.6.40/lib pngwriter.c -lpng

clean:
	rm -rf mandel_parallel
