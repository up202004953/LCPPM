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

int main(int argc, char* argv[]) {
  FILE* imgIn1; FILE* imgIn2; FILE* imgOut;
  int posx, posy;

  if (argc > 6) {
    fprintf(stderr, "Uso indevido: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  if (argc > 5) {
    imgOut = fopen(*(argv+5), "w");
    if (imgOut == NULL) {
      fprintf(stderr, "Impossivel abrir o ficheiro para escrita: %s\n", *(argv+5));
      return 0;
    }
  }
  else imgOut = stdout;

  if (argc > 3) {
    imgIn2 = fopen(*(argv+3), "r");
    if (imgIn2 == NULL) {
      fprintf(stderr, "Ficheiro não encontrado: %s\n", *(argv+3));
      return 0;
    }
  }
  else imgIn2 = stdin;

  if (argc > 4) {
    imgIn1 = fopen(*(argv+4), "r");
    if (imgIn1 == NULL) {
      fprintf(stderr, "Ficheiro não encontrado: %s\n", *(argv+4));
      return 0;
    }
  }
  else imgIn1 = stdin;

  if (argc > 2) {
    posx = number(*(argv+1)); posy = number(*(argv+2));
    if (posx == -1 || posy == -1) {
      fprintf(stderr, "Uso indevido: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
      return 0;
    }
  }
  else {
    fprintf(stderr, "Uso indevido: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

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

  int col = getInt(imgIn1);
  int col2 = getInt(imgIn2);
  fprintf(imgOut, "%d ", col);

  int lin = getInt(imgIn1);
  int lin2 = getInt(imgIn2);
  fprintf(imgOut, "%d\n", lin);

  int maxRgb = getInt(imgIn1);
  getInt(imgIn2);
  fprintf(imgOut, "%d\n", maxRgb);

  COLOR matrix1[lin][col];
  COLOR matrix2[lin2][col2];
  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      matrix1[i][j].r = getInt(imgIn1);
      matrix1[i][j].g = getInt(imgIn1);
      matrix1[i][j].b = getInt(imgIn1);
    }
  }
  for(int i = 0; i < lin2; i++) {
    for(int j = 0; j < col2; j++) {
      matrix2[i][j].r = getInt(imgIn2);
      matrix2[i][j].g = getInt(imgIn2);
      matrix2[i][j].b = getInt(imgIn2);
    }
  }

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      if (!(i == 0 && j == 0)) fprintf(imgOut, "\n");
      if (i >= posy && (i-posy) < lin2 && j >= posx && (j-posx) < col2) {
        fprintf(imgOut, "%d %d %d", matrix2[i-posy][j-posx].r, matrix2[i-posy][j-posx].g, matrix2[i-posy][j-posx].b);
      }
      else {
        fprintf(imgOut, "%d %d %d", matrix1[i][j].r, matrix1[i][j].g, matrix1[i][j].b);
      }
    }
  }
  fprintf(imgOut, "\n");
  fclose(imgIn1); fclose(imgOut);
  return 0;
}
