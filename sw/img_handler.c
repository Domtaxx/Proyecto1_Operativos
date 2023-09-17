#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int width;
int height;
int channels;
const char* input_file_path = "img/InputImages/input_image0.jpg"; // Ruta a tu imagen
const char* output_file_path = "img/OutputImages/output_image.jpg"; // Ruta a tu imagen

unsigned char* getImage() {

    unsigned char* image_data = stbi_load(input_file_path, &width, &height, &channels, 1);

    if (!image_data) {
        printf("Error al cargar la imagen.\n");
        // return 1;
    }
    return image_data;
}

void freeImage(unsigned char* image_data){
     stbi_image_free(image_data);
}

void saveImage(unsigned char* image_data){
    if(stbi_write_jpg(output_file_path, width, height, 1, image_data, 100) == 0){
        printf("Failed to save image\n");
    };
}
