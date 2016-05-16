# tesseract-demo
This is a demo of a small app to try and parallelize ocr operations on *tiff* files.

I've been asked to investigate on the possibility to speed-up tesseract ocr engine, which is great and free from google, but can slow down dramatically a complex workflow where tons of multi-paged documents have to be converted to plain text.
Ocr operations can be thought to be very suitable to be parallelised, and even to be implemented on heterogeneous architectures (CPU + GPU): for example, see http://www.cs.uky.edu/~raphael/grad/keepingCurrent/reed-ocr.pdf

Tesseract's team itself was putting effort in running in a heterogeneous environment, getting involved ATI's people to translate the engine in OpenCl; I've taken contact with them after some trials, and I realised there still was much to do.

So I profiled tesseract to see whether there was room for a significative speed-up by writing the only ocr functions with **CUDA**.
The results have suggested not to proceed in that direction, since the execution time was spread among many functions (I can share a small analysis carried out via valgrind's tool callgrind coupled with a graphic interface - Kcachegrind if you're interested).

So I decided to proceed this way: 
* one parallelisation can be made running in a multiprocess fashion, one instance of tesseract per page of a document;
* the other - more fun! - was trying and use tesseract APIs to use multi-threading on a single page, doing ocr one thread per line (or per word);
* couple the two things together.

I also use vectorisation, with no significative results.

Results where quite nice, showing a good scaling efficiency up to 4 threads, and then degradating; the multi-process parallelisation behaved as expected, the speed-up being almost linear with the number of pages (and then of instances) up to a 10 pages documents - more or less.

The small code here is organised very easily: the `.sh` file is meant to be run with a tiff file as argument, it will read the number of pages in the tiff file, then it will launch as many instances of `tessapi-quality` as the number of pages.
`tessapi-quality` is the executable generated via the `makefile` (just type `make`) by compiling and linking `tessapi-quality.cpp`.

Don't know if I'm going to make other gym with this code soon - I know it deserves **hard gym!** - maybe if I find anyone interested..
