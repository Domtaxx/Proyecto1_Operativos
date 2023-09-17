#include <stdio.h>
#include <stdlib.h>
#include "img_handler.c"


void applyLaplacianFilter(unsigned char* input, unsigned char* output, int width, int height) {
    int kernel[3][3] = {
        {0,  1,  0},
        {1, -4,  1},
        {0,  1,  0}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sum = 0;
            int index = y*width + x;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int pixelValue = input[(y + ky) * width + (x + kx)];
                    sum += pixelValue * kernel[ky + 1][kx + 1];
                }
            }
            sum = sum > 255 ? 255 : (sum < 0 ? 0 : sum); // Clamp values to [0, 255]
            output[index] = (unsigned char)sum;
        }
    }
}   


int main() {

    unsigned char* image;
    unsigned char* lap_image;
    image = getImage();
    lap_image = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    
    applyLaplacianFilter(image, lap_image, width, height);
    saveImage(lap_image);


    freeImage(image);
    free(lap_image);
    return 0;
}