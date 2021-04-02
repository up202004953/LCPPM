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

  char* Scol = getString(imgIn);;
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
      matrix[i][j] = calloc(MAXSTR,sizeof(char));
    }
  }
  char color[MAXSTR];

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      color[0] = '\0';
      strcat(color, getString(imgIn)); strcat(color, " ");
      strcat(color, getString(imgIn)); strcat(color, " ");
      strcat(color, getString(imgIn));
      strcpy(matrix[i][j],color);
    }
  }

  for(int i = lin-1; i >= 0; i--) {
    for(int j = 0; j < col; j++) {
      if (!(i == lin-1 && j == 0)) fprintf(imgOut, "\n");
      fprintf(imgOut, "%s ", matrix[i][j]);
      free(matrix[i][j]);
    }
  }

  fclose(imgIn); fclose(imgOut);
  return 0;
}
