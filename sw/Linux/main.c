// #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img_handler.c"
#include "modular_expo.c"
#include "rsa.c"
#include "laplacian_filter.c"
#include "readFile.c"

char* nombreArchivo = "ejemploEncoded.txt";
const char* output_file_path_lap = "output_image_lap.jpg";
const char* output_file_path_original = "output_image_original.jpg";

unsigned int width;
unsigned int height;
int num_valores;
unsigned int* valores;


void removeWidthAndHighFrom(unsigned int* valores){
  valores = valores + 2;
  num_valores = num_valores - 2;
}

void setWidthHeight(unsigned int* valores){
  width = single_decrypt(valores[0], 2565, 3901);
  height = single_decrypt(valores[1], 2565, 3901);
  removeWidthAndHighFrom(valores);
}


int main() {

  leerArchivo(nombreArchivo, &valores, &num_valores);

  setWidthHeight(valores);

  unsigned char* decry_msg = decrypt(valores+2, num_valores-2, 2565, 3901);
  saveImage(output_file_path_original, decry_msg, width, height);
  free(valores); //Ya no se usan los valores

  unsigned char* lap_image = (unsigned char*)malloc(width * height * sizeof(unsigned char));
  applyLaplacianFilter(decry_msg, lap_image, width, height);
  free(decry_msg); //Ya no se usa el mensaje desencriptado
  saveImage(output_file_path_lap, lap_image, width, height);
  free(lap_image);

  return 0;
}