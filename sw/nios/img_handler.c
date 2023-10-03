#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


// int width;
// int height;
// int channels;
// const char* input_file_path = "img/InputImages/dragon_ball.jpeg"; // Ruta a tu imagen
// const char* output_file_path = "img/OutputImages/output_image.jpg"; // Ruta a tu imagen


// void printArray(unsigned char* arr, int len)
// {
//     int i;
//     for (i = 0; i < len; i++){
//         printf("%d ", arr[i]);
//     }
// }

// unsigned char* getImage() {

//     unsigned char* image_data = stbi_load(input_file_path, &width, &height, &channels, 1);

//     printArray(image_data, 100);

//     if (!image_data) {
//         printf("Error al cargar la imagen.\n");
//         // return 1;
//     }
//     return image_data;
// }

void saveImage(const char* file_path, unsigned char* image_data, int width, int height){
    if(stbi_write_jpg(file_path, width, height, 1, image_data, 100) == 0){
        printf("Failed to save image\n");
    };
}
