#compiler
CC = g++
TLIB = -I/usr/local/include/tesseract
LLIB = -L/usr/local/lib
OPT = -llept -ltesseract -O3
VEC = -ftree-vectorize -ftree-vectorizer-verbose=1
PAR = -fopenmp 

all: tessapi-quality

tessapi-quality: tessapi-quality.cpp
	$(CC) -o $@ $^ $(TLIB) $(LLIB) $(OPT) $(PAR)

clean:
	@echo "... removing bin and swap"
	rm -rf tessapi-quality *~
