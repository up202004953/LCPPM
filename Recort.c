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
  char* buff = calloc(512,sizeof(char));
  fscanf(f, "%s", buff);
  while(*buff == '#') {
    readEOL(f);
    fscanf(f, "%s", buff);
  }
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
  int xmin,ymin,xmax,ymax;
  if (argc > 7) {
    fprintf(stderr, "Uso indevido: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  if (argc > 6) {
    imgOut = fopen(*(argv+6), "w");
    if (imgOut == NULL) {
      fprintf(stderr, "Impossivel abrir o ficheiro para escrita: %s\n", *(argv+6));
      return 0;
    }
  }
  else imgOut = stdout;

  if (argc > 5) {
    imgIn = fopen(*(argv+5), "r");
    if (imgIn == NULL) {
      fprintf(stderr, "Ficheiro não encontrado: %s\n", *(argv+5));
      return 0;
    }
  }
  else imgIn = stdin;

  if (argc > 4) {
    xmin = number(*(argv+1)); ymin = number(*(argv+2));
    xmax = number(*(argv+3)); ymax = number(*(argv+4));
  }

  char* rgb = getString(imgIn); //P3
  if (strcmp(rgb,"P3")) {
    fprintf(stderr, "O ficheiro de entrada não tem valores validos\n");
    return 0;
  }
  fprintf(imgOut, "%s\n", rgb);
  free(rgb);

  int col = getInt(imgIn);
  if (xmax > col) xmax = col-1;
  fprintf(imgOut, "%d ", xmax-xmin+1);

  int lin = getInt(imgIn);
  if (ymax > lin) ymax = lin-1;
  fprintf(imgOut, "%d\n", ymax-ymin+1);

  int maxRgb = getInt(imgIn);
  fprintf(imgOut, "%d", maxRgb);

  COLOR matrix[lin][col];
  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      matrix[i][j].r = getInt(imgIn);
      matrix[i][j].g = getInt(imgIn);
      matrix[i][j].b = getInt(imgIn);
    }
  }

  for(int i = ymin; i <= ymax; i++) {
    for(int j = xmin; j <= xmax; j++) {
      fprintf(imgOut, "\n%d %d %d", matrix[i][j].r, matrix[i][j].g, matrix[i][j].b);
    }
  }

  fprintf(imgOut, "\n");
  fclose(imgIn); fclose(imgOut);
  return 0;
}
