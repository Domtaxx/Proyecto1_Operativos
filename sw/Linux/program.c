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

void setWidthHeight(unsigned int* valores, int d, int n){
  width = single_decrypt(valores[0], d, n);
  height = single_decrypt(valores[1], d, n);
  removeWidthAndHighFrom(valores);
}


int run_program(int d, int n) {

  leerArchivo(nombreArchivo, &valores, &num_valores);

  setWidthHeight(valores, d, n);

  unsigned char* decry_msg = decrypt(valores, num_valores, d, n);
  saveImage(output_file_path_original, decry_msg, width, height);
  free(valores); //Ya no se usan los valores

  unsigned char* lap_image = (unsigned char*)malloc(width * height * sizeof(unsigned char));
  applyLaplacianFilter(decry_msg, lap_image, width, height);
  free(decry_msg); //Ya no se usa el mensaje desencriptado
  saveImage(output_file_path_lap, lap_image, width, height);
  free(lap_image);

  return 0;
}