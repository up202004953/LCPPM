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

int main(int agrc, char* argv[]) {
  FILE* imgIn; FILE* imgOut;

  imgIn = fopen(*(argv+1), "r"); imgOut = fopen(*(argv+2), "w");
  if (imgIn == NULL) {
    fprintf(imgOut,"Ficheiro não encontrado: %s", *(argv+1));
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

  for(int i = 0; i < lin; i++) {
    for(int j = col-1; j >= 0; j--) {
      if (!(i == 0 && j == col-1)) fprintf(imgOut, "\n");
      fprintf(imgOut, "%s ", matrix[i][j]);
      free(matrix[i][j]);
    }
  }

  fclose(imgIn); fclose(imgOut);
  return 0;
}
