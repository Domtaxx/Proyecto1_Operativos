#include <stdio.h>
#include <stdlib.h>
#include "img_handler.c"

int main() {

    unsigned char* image;
    unsigned char* lap_image;
    image = getImage();
    lap_image = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    
    // applyLaplacianFilter(image, lap_image, width, height);
    saveImage(image);


    freeImage(image);
    free(lap_image);
    return 0;
}