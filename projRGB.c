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

char* add(char* buff, char* color, char* maxRgb) {
  char* str = malloc(3*sizeof(char));
  char* Saux = malloc(3*sizeof(char));
  int max = atoi(maxRgb); int aux;

  aux = atoi(buff) + atoi(color);
  if (aux > max) {
    sprintf(Saux,"%s", maxRgb);
  }
  else if (aux < 0) {
    sprintf(Saux,"%d", 0);
  }
  else {
    sprintf(Saux,"%d", aux);
  }
  strcat(str, Saux);

  return str;
}

int main(int argc, char* argv[]) {
  char* r = *(argv+1); char* g = *(argv+2); char* b = *(argv+3);

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
    char* r = *(argv+1); char* g = *(argv+2); char* b = *(argv+3);
    if (number(r) == -1 || number(g) == -1 || number(b) == -1) {
        printf("Uso indevido: %s <dR> <dG> <dB> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
        return 0;
    }
  }
  else {
    printf("Uso indevido: %s <dR> <dG> <dB> [<ficheiro de entrada> [ficheiro de saida]]\n", *argv);
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
      matrix[i][j] = calloc(MAXSTR,sizeof(char));
    }
  }
  char Scolor[MAXSTR];
  char* color = malloc(3*sizeof(char));

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      Scolor[0] = '\0';
      color = getString(imgIn); strcpy(color,add(color,r,maxRgb));
      strcat(Scolor, color); strcat(Scolor, " ");
      color = getString(imgIn); strcpy(color,add(color,g,maxRgb));
      strcat(Scolor, color); strcat(Scolor, " ");
      color = getString(imgIn); strcpy(color,add(color,b,maxRgb));
      strcat(Scolor, color);
      strcpy(matrix[i][j],Scolor);
    }
  }

  for(int i = 0; i < lin; i++) {
    for(int j = 0; j < col; j++) {
      if (!(i == 0 && j == 0)) fprintf(imgOut, "\n");
      fprintf(imgOut, "%s ", matrix[i][j]);
      free(matrix[i][j]);
    }
  }

  free(maxRgb);
  fclose(imgIn); fclose(imgOut);
  return 0;
}
