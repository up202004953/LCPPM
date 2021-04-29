#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 12

typedef struct color {
  int r;
  int g;
  int b;
} COLOR;

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

int main(int argc, char* argv[]) {
  FILE* imgIn; FILE* imgOut;
  if (argc > 3) {
    fprintf(stderr, "Uso indevido: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  if (argc > 2) {
    imgOut = fopen(*(argv+2), "w");
    if (imgOut == NULL) {
      fprintf(stderr, "Impossivel abrir o ficheiro para escrita: %s\n", *(argv+2));
      return 0;
    }
  }
  else imgOut = stdout;

  if (argc > 1) {
    imgIn = fopen(*(argv+1), "r");
    if (imgIn == NULL) {
      fprintf(stderr, "Ficheiro não encontrado: %s\n", *(argv+1));
      return 0;
    }
  }
  else imgIn = stdin;


  char* rgb = getString(imgIn); //P3
  if (strcmp(rgb,"P3")) {
    fprintf(stderr, "O ficheiro de entrada não tem valores validos\n");
    return 0;
  }
  fprintf(imgOut, "%s\n", rgb);
  free(rgb);

  int col = getInt(imgIn);
  int lin = getInt(imgIn);

  fprintf(imgOut, "%d ", lin);
  fprintf(imgOut, "%d\n", col);

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

  for(int j = 0; j < col; j++) {
    for(int i = lin - 1; i >= 0; i--) {
      if (!(i == lin - 1 && j == 0)) fprintf(imgOut, "\n");
      fprintf(imgOut, "%d %d %d", matrix[i][j].r, matrix[i][j].g, matrix[i][j].b);
    }
  }

  fclose(imgIn); fclose(imgOut);
  return 0;
}
