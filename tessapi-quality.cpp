#include <stdlib.h>
#include <iostream>
//#include </usr/local/include/leptonica/allheaders.h>
#include </usr/local/Cellar/leptonica/1.71_1/include/leptonica/allheaders.h>
#include <omp.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
// include tesseract baseapi.h with the right path
#include </Users/satavtar/github/tesseract/api/baseapi.h>

void getCompImage(const char* filename, int page){

    Pix* image;
    struct timeval start, end;
    int cpu, status;
    double delta;
    Pixa** pixa;
    int** blockids;
    /*
     * Read total number of pages in multiTiff input files into npages variable

     int npages;
     FILE *fp, *fo;
     fp = fopen(filename, "rb");
     if (fp == NULL) {
     printf("Image file cannot be opened!\n");
     exit(-1);
     }
     tiffGetCount(fp, &npages);
     fclose(fp);
    // ok, total n. of pages in npages variable

    //fo = fopen("output.txt", "a+t");
    */
    printf("Running line by line example..\n\n\n");
    image = pixReadTiff(filename, page);

    // set number of threads to use
    int num_threads = 4;
    omp_set_num_threads(num_threads);

    gettimeofday(&start, NULL);
#pragma omp parallel 
    {
        tesseract::TessBaseAPI *papi = new tesseract::TessBaseAPI();
        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (papi->Init(NULL, "eng")) {
            fprintf(stdout, "Could not initialize tesseract.\n");
            exit(1);
        }

        papi->SetImage(image);
        Boxa* boxes = papi->GetComponentImages(tesseract::RIL_TEXTLINE, true, pixa, blockids);
        //  printf("Found %d textline image components.\n", boxes->n);
#pragma omp for schedule(dynamic, 1) 
        for (int i = 0; i < boxes->n; i++) {
            BOX* box = boxaGetBox(boxes, i, L_CLONE);

            papi->SetRectangle(box->x, box->y, box->w, box->h);
            char* ocrResult = papi->GetUTF8Text();
            int my_thread = omp_get_thread_num();
            //int conf = papi->MeanTextConf();
            fprintf(stdout, "Thread %d, Page %d: Box[%d], text: %s",
                         my_thread, page,          i, ocrResult);
        }
        // Destroy used object and release memory
        papi->End();
    }

    gettimeofday(&end, NULL);
    delta = ((end.tv_sec  - start.tv_sec) * 1000000u + 
            end.tv_usec - start.tv_usec) / 1.e6;
    fprintf(stdout, "elapsed time: %f\n\n", delta);

    pixDestroy(&image);
}


int main(int argc, char *argv[])
{

    if (argc != 3){
        printf("Type the (tiff) file name to OCRize\n"
                "Then the page in the file to OCRize (first page=0, second=1 etc..)\n\n");
        exit(-1);
    }

    int page = atoi(argv[2]);

    getCompImage(argv[1], page);

    return 0;
}
