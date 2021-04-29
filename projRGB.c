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
  if (!(*str == '+' || *str == '-' || (*str >= '0' && *str <= '9'))) return -1;
  for(int i = 1; *(str+i) != '\0'; i++) {
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

int add(int num, int color, int maxRgb) {
  int aux = num + color;
  if (aux > maxRgb) return maxRgb;
  else if (aux < 0) return 0;
  else return aux;
}

int main(int argc, char* argv[]) {
  int dr; int dg; int db;

  FILE* imgIn; FILE* imgOut;

  if (argc > 6) {
    printf("Uso indevido: %s <dR> <dG> <dB> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  if (argc > 5) {
    imgOut = fopen(*(argv+5), "w");
    if (imgOut == NULL) {
      printf("Impossivel abrir o ficheiro para escrita: %s\n", *(argv+5));
      return 0;
    }
  }
  else imgOut = stdout;

  if (argc > 4) {
    imgIn = fopen(*(argv+4), "r");
    if (imgIn == NULL) {
      printf("Ficheiro não encontrado: %s\n", *(argv+4));
      return 0;
    }
  }
  else imgIn = stdin;

  if (argc > 3) {
    dr = number(*(argv+1)); dg = number(*(argv+2)); db = number(*(argv+3));
    if (dr == -1 || dg == -1 || db == -1) {
        printf("Uso indevido: %s <dR> <dG> <dB> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
        return 0;
    }
  }
  else {
    printf("Uso indevido: %s <dR> <dG> <dB> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
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
      fprintf(imgOut, "%d %d %d",
              add(matrix[i][j].r,dr,maxRgb),
              add(matrix[i][j].g,dg,maxRgb),
              add(matrix[i][j].b,db,maxRgb));
    }
  }

  fclose(imgIn); fclose(imgOut);
  return 0;
}
