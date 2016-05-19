#compiler
CC = g++-5 #clang-omp++
TLIB = -I/usr/local/include/tesseract
LLIB = -L/usr/local/lib
OPT = -llept -ltesseract 
VEC = -ftree-vectorize -ftree-vectorizer-verbose=1
PAR = -fopenmp 
DEBUG = -g

all: tessapi-quality

tessapi-quality: tessapi-quality.cpp
	$(CC) -o $@ $^ $(TLIB) $(LLIB) $(OPT) $(PAR) #$(DEBUG)

clean:
	@echo "... removing bin and swap"
	rm -rf tessapi-quality *~
