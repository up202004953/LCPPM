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

int getMin(int a, int b) {
  if (a > b) return b;
  return a;
}

int main(int argc, char* argv[]) {
  FILE* imgIn1; FILE* imgIn2; FILE* imgOut;
  if (argc > 4) {
    fprintf(stderr, "Uso indevido: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  if (argc > 3) {
    imgOut = fopen(*(argv+3), "w");
    if (imgOut == NULL) {
      fprintf(stderr, "Impossivel abrir o ficheiro para escrita: %s\n", *(argv+3));
      return 0;
    }
  }
  else imgOut = stdout;

  if (argc > 2) {
    imgIn2 = fopen(*(argv+2), "r");
    if (imgIn2 == NULL) {
      fprintf(stderr, "Ficheiro não encontrado: %s\n", *(argv+2));
      return 0;
    }
  }
  else imgIn2 = stdin;

  if (argc > 1) {
    imgIn1 = fopen(*(argv+1), "r");
    if (imgIn1 == NULL) {
      fprintf(stderr, "Ficheiro não encontrado: %s\n", *(argv+1));
      return 0;
    }
  }
  else imgIn1 = stdin;

  char* rgb = getString(imgIn1); //P3
  if (strcmp(rgb,"P3")) {
    fprintf(stderr, "O ficheiro de entrada não tem valores validos\n");
    return 0;
  }
  rgb = getString(imgIn2);
  if (strcmp(rgb,"P3")) {
    fprintf(stderr, "O ficheiro de entrada não tem valores validos\n");
    return 0;
  }
  fprintf(imgOut, "%s\n", rgb);
  free(rgb);

  int col1 = getInt(imgIn1);
  int col = col1 + getInt(imgIn2);
  fprintf(imgOut, "%d ", col);

  int lin = getMin(getInt(imgIn1), getInt(imgIn2));
  fprintf(imgOut, "%d\n", lin);

  int maxRgb = getInt(imgIn1);
  getInt(imgIn2);
  fprintf(imgOut, "%d\n", maxRgb);

  COLOR matrix[lin][col];
  for(int i = 0; i < lin; i++) {
    int j = 0;
    for(; j < col1; j++) {
      matrix[i][j].r = getInt(imgIn1);
      matrix[i][j].g = getInt(imgIn1);
      matrix[i][j].b = getInt(imgIn1);
    }
    for(; j < col; j++) {
      matrix[i][j].r = getInt(imgIn2);
      matrix[i][j].g = getInt(imgIn2);
      matrix[i][j].b = getInt(imgIn2);
    }
  }

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      if (!(i == 0 && j == 0)) fprintf(imgOut, "\n");
      fprintf(imgOut, "%d %d %d", matrix[i][j].r, matrix[i][j].g, matrix[i][j].b);
    }
  }

  fclose(imgIn1); fclose(imgOut);
  return 0;
}
