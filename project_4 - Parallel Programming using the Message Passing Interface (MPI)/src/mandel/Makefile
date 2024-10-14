CC = mpicc -ld_classic
CFLAGS = -O3 -I/opt/homebrew/Cellar/libpng/1.6.40/include/libpng16 -L/opt/homebrew/Cellar/libpng/1.6.40/lib -lpng16
SOURCES = mandel_mpi.c pngwriter.c
TARGET = mandel_mpi

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)