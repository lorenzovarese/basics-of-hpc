all: mandel_seq

mandel_seq: 
	gcc -o mandel_seq -I/opt/homebrew/Cellar/libpng/1.6.40/include -L/opt/homebrew/Cellar/libpng/1.6.40/lib -O3 mandel_seq.c pngwriter.c -lpng

clean:
	rm -rf mandel_seq
