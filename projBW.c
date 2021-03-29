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

int main(int agrc, char* argv[]) {
  int thresh = atoi(*(argv+1));

  FILE* imgIn; FILE* imgOut;

  imgIn = fopen(*(argv+2), "r"); imgOut = fopen(*(argv+3), "w");
  if (imgIn == NULL) {
    fprintf(imgOut,"Ficheiro não encontrado: %s", *(argv+2));
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
