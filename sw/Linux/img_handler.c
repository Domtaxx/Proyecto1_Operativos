#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


void saveImage(const char* file_path, unsigned char* image_data, int width, int height){
    if(stbi_write_jpg(file_path, width, height, 1, image_data, 100) == 0){
        printf("Failed to save image\n");
    };
}
