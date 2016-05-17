#compiler
CC = clang-omp++
TLIB = -I/usr/local/include/tesseract
LLIB = -L/usr/local/lib
OPT = -llept -ltesseract -O3
VEC = -ftree-vectorize -ftree-vectorizer-verbose=1
PAR = -fopenmp 
DEBUG = -E -C

all: tessapi-quality

tessapi-quality: tessapi-quality.cpp
	$(CC) -o $@ $^ $(TLIB) $(LLIB) $(OPT) $(PAR) $(DEBUG)

clean:
	@echo "... removing bin and swap"
	rm -rf tessapi-quality *~
