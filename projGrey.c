#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 12

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

char* convert(char* r, char* g, char* b) {
  char* str = calloc(MAXSTR,sizeof(char));
  int aux;
  aux = (float) (atoi(r)*(0.2126f) + atoi(g)*(0.7152f) + atoi(b)*(0.0722f));
  sprintf(str, "%d %d %d", aux, aux, aux);
  return str;
}

int main(int argc, char* argv[]) {
  FILE* imgIn; FILE* imgOut;

  if (argc > 3) {
    printf("Uso indevido: %s [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }

  if (argc > 2) {
    imgOut = fopen(*(argv+2), "w");
    if (imgOut == NULL) {
      printf("Impossivel abrir o ficheiro para escrita: %s\n", *(argv+2));
      return 0;
    }
  }
  else imgOut = stdout;

  if (argc > 1) {
    imgIn = fopen(*(argv+1), "r");
    if (imgIn == NULL) {
      printf("Ficheiro não encontrado: %s", *(argv+1));
      return 0;
    }
  }
  else imgIn = stdin;

  char* rgb = getString(imgIn);
  fprintf(imgOut, "%s\n", rgb);
  free(rgb);

  char* Scol = getString(imgIn);
  fprintf(imgOut, "%s ", Scol);
  int col = atoi(Scol);
  free(Scol);

  char* Slin = getString(imgIn);
  fprintf(imgOut, "%s\n", Slin);
  int lin = atoi(Slin);
  free(Slin);

  char* maxRgb = getString(imgIn);
  fprintf(imgOut, "%s\n", maxRgb);
  free(maxRgb);

  char* matrix[lin][col];

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      char* r = getString(imgIn); char* g = getString(imgIn); char* b = getString(imgIn);
      matrix[i][j] = convert(r,g,b);
      free(r); free(g); free(b);
    }
  }

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      if (!(i == 0 && j == 0)) fprintf(imgOut, "\n");
      fprintf(imgOut, "%s ", matrix[i][j]);
      free(matrix[i][j]);
    }
  }

  fclose(imgIn); fclose(imgOut);
  return 0;
}
