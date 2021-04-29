#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 12

typedef struct color {
  int r;
  int g;
  int b;
} COLOR;

int number(char* str) {
  for(int i = 0; *(str+i) != '\0'; i++) {
    if (!(*(str+i) >= '0' && *(str+i) <= '9')) return -1;
  }
  return atoi(str);
}

void readEOL(FILE* f) {
  char buff;
  do {
    fscanf(f, "%c", &buff);
  } while (buff != '\n');
}

char* getString(FILE* f) {
  char* buff = calloc(512,sizeof(char));
  fscanf(f, "%s", buff);
  while(*buff == '#') {
    readEOL(f);
    fscanf(f, "%s", buff);
  }
  return buff;
}

int getInt(FILE* f) {
  char* buff = getString(f);
  for(int i = 0; *(buff+i) != '\0'; i++) {
    if (!(*buff >= '0' && *buff <= '9')) {
      fprintf(stderr, "O ficheiro de entrada não tem valores validos\n");
      exit(0);
    }
  }
  return atoi(buff);
}

int convert(int r, int g, int b, int thresh, int maxRgb) {
  int aux = (float) (r*(0.2126) + g*(0.7152) + b*(0.0722));
  if (aux > thresh) return maxRgb;
  return 0;
}

int main(int argc, char* argv[]) {
  int thresh;
  FILE* imgIn; FILE* imgOut;

  if (argc > 4) {
    printf("Uso indevido: %s <limite> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  if (argc > 3) {
    imgOut = fopen(*(argv+3), "w");
    if (imgOut == NULL) {
      printf("Impossivel abrir o ficheiro para escrita: %s\n", *(argv+3));
      return 0;
    }
  }
  else imgOut = stdout;

  if (argc > 2) {
    imgIn = fopen(*(argv+2), "r");
    if (imgIn == NULL) {
      printf("Ficheiro não encontrado: %s\n", *(argv+2));
      return 0;
    }
  }
  else imgIn = stdin;

  if (argc > 1) {
    thresh = number(*(argv+1));
    if (thresh == -1) {
      fprintf(stderr, "O ficheiro de entrada não tem valores validos\n");
      return 0;
    }
  }
  else {
    printf("Uso indevido: %s <limite> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  char* rgb = getString(imgIn);
  if (strcmp(rgb,"P3")) {
    fprintf(stderr, "O ficheiro de entrada não tem valores validos\n");
    return 0;
  }
  fprintf(imgOut, "%s\n", rgb);
  free(rgb);

  int col = getInt(imgIn);
  fprintf(imgOut, "%d ", col);

  int lin = getInt(imgIn);
  fprintf(imgOut, "%d\n", lin);

  int maxRgb = getInt(imgIn);
  fprintf(imgOut, "%d\n", maxRgb);

  COLOR matrix[lin][col];
  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      matrix[i][j].r = getInt(imgIn);
      matrix[i][j].g = getInt(imgIn);
      matrix[i][j].b = getInt(imgIn);
    }
  }

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      if (!(i == 0 && j == 0)) fprintf(imgOut, "\n");
      int aux = convert(matrix[i][j].r, matrix[i][j].g, matrix[i][j].b, thresh, maxRgb);
      fprintf(imgOut, "%d %d %d", aux, aux, aux);
    }
  }

  fclose(imgIn); fclose(imgOut);
  return 0;
}
