#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 12

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

int convert(char* r, char* g, char* b) {
  return (atoi(r)*(0.2126f) + atoi(g)*(0.7152f) + atoi(b)*(0.0722f));
}


char* convertBW(char* r, char* g, char* b, int thresh, int maxRgb) {
  char* str = calloc(MAXSTR,sizeof(char));
  int aux = convert(r,g,b);
  if (aux > thresh) {
    sprintf(str, "%d %d %d", maxRgb, maxRgb, maxRgb);
  }
  else {
    sprintf(str, "0 0 0");
  }
  return str;
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
    int thresh = number(*(argv+1));
    if (thresh == -1) {
        printf("Uso indevido: %s <limite> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
        return 0;
    }
  }
  else {
    printf("Uso indevido: %s <limite> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
    return 0;
  }
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

  char* matrix[lin][col];

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      char* r = getString(imgIn); char* g = getString(imgIn); char* b = getString(imgIn);
      matrix[i][j] = convertBW(r,g,b,thresh,atoi(maxRgb));
      free(r); free(g); free(b);
    }
  }

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      if (!(i == 0 && j == 0)) fprintf(imgOut, "\n");
      fprintf(imgOut, "%s", matrix[i][j]);
      free(matrix[i][j]);
    }
  }

  fclose(imgIn); fclose(imgOut);
  return 0;
}
