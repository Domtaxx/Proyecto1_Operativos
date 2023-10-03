#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

void leerArchivo(char* filename, int** arr, int* size) {
  FILE* fp;
  int i = 0, num;
  
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return;
  }
  
  *arr = malloc(sizeof(int)); 
  while (fscanf(fp, "%d,", &num) > 0) {
    (*arr)[i] = num;
    i++;
    *arr = realloc(*arr, (i + 1) * sizeof(int));
  }
  fclose(fp);

  *size = i;
}