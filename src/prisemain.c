#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "../lib/def.h"
#include "../lib/nrio.h"
#include "../lib/nrarith.h"
#include "../lib/nralloc.h"

#define ELT_X 3
#define ELT_Y 3
int main(int argc, char const *argv[]) {

  detect_mvt();
  return 0;
}

void prisemain() {
  long nrh,nrl,nch,ncl;
  byte **R, **R2;
  int i, j;

  R = LoadPGM_bmatrix("../res/Test/carreTrou.pgm",&nrl,&nrh,&ncl,&nch);
  R2 = bmatrix(nrl, nrh, ncl, nch);

  dilate(R, R2, nrh, nrl, nch, ncl);
  dilate(R2, R, nrh, nrl, nch, ncl);
  dilate(R, R2, nrh, nrl, nch, ncl);
  dilate(R2, R, nrh, nrl, nch, ncl);
  dilate(R, R2, nrh, nrl, nch, ncl);
  dilate(R2, R, nrh, nrl, nch, ncl);
  dilate(R, R2, nrh, nrl, nch, ncl);
  dilate(R2, R, nrh, nrl, nch, ncl);
  dilate(R, R2, nrh, nrl, nch, ncl);
  dilate(R2, R, nrh, nrl, nch, ncl);
  dilate(R, R2, nrh, nrl, nch, ncl);

  SavePGM_bmatrix(R2,nrl,nrh,ncl,nch,"./prisemain.pgm");
  free_bmatrix(R, nrl, nrh, ncl, nch);
  free_bmatrix(R2, nrl, nrh, ncl, nch);
}

void detect_mvt() {
  long nrl, nrh, ncl, nch;
  rgb8 **imgs[5]; //Sur 5 images
  byte **test;
  int i;
  char *img_prefix = "../res/Sequences/Fomd/ppm/fomd00";
  char *img_suffix = ".ppm";
  char img_path[50];
  for(i=0; i<5; i++) {
    sprintf(img_path, "%s%d%s", img_prefix, i+1, img_suffix);
    imgs[i] = LoadPPM_rgb8matrix(img_path, &nrl, &nrh, &ncl, &nch);
  }
  test = bmatrix(nrl, nrh, ncl, nch);
  //get_seuillee(imgs[0], test, 127, nrh, nrl, nch, ncl);
  //SavePGM_bmatrix(test, nrl, nrh, ncl, nch, "./prisemain2.pgm");
  /*free_rgb8matrix(imgs[0], nrl, nrh, ncl, nch);
  free_rgb8matrix(imgs[1], nrl, nrh, ncl, nch);
  free_rgb8matrix(imgs[2], nrl, nrh, ncl, nch);
  free_rgb8matrix(imgs[3], nrl, nrh, ncl, nch);
  free_rgb8matrix(imgs[4], nrl, nrh, ncl, nch);*/
  free_bmatrix(test, nrl, nrh, ncl, nch);
}
//TODO: struct for structuring element
void erode(byte **from, byte **to, long nrh, long nrl, long nch, long ncl) {
  int i, j;

  for(i=1; i<nrh; i++) {
    for(j=1; j<nch; j++) {
      if(from[i-1][j-1] == 0 || from[i][j-1] == 0 || from[i+1][j-1] == 0 ||
         from[i-1][j] == 0 || from[i][j] == 0 || from[i+1][j] == 0 ||
         from[i-1][j+1] == 0 || from[i][j+1] == 0 || from [i+1][j+1] == 0) { //Si un truc du voisinage est ==0
           to[i][j] = 0;
         } else {
           to[i][j] = 255;
         }
    }
  }
}

void dilate(byte **from, byte **to, long nrh, long nrl, long nch, long ncl) {
  int i, j;

  for(i=1; i<nrh; i++) {
    for(j=1; j<nch; j++) {
      if(from[i-1][j-1] == 255 || from[i][j-1] == 255 || from[i+1][j-1] == 255 ||
         from[i-1][j] == 255 || from[i][j] == 255 || from[i+1][j] == 255 ||
         from[i-1][j+1] == 255 || from[i][j+1] == 255 || from [i+1][j+1] == 255) { //Si un truc du voisinage est ==0
           to[i][j] = 255;
         } else {
           to[i][j] = 0;
         }
    }
  }
}

void get_nivgris(rgb8 **from, byte **to, long nrh, long nrl, long nch, long ncl) {
  int i, j;
  for (i = 0 ; i < nrh ; i++) {
    for(j = 0 ; j < nch ; j++) {
      to[i][j] = from[i][j].r/3 + from[i][j].g/3 + from[i][j].b/3;
    }
  }
}
void get_seuillee(rgb8 **from, byte **to, int seuil, long nrh, long nrl, long nch, long ncl) {
  int i, j;
  get_nivgris(from, to, nrh, nrl, nch, ncl);
  for (i = 0 ; i < nrh ; i++) {
    for(j = 0 ; j < nch ; j++) {
      if(to[i][j] > seuil) {
        to[i][j] = 255;
      } else {
        to[i][j] = 0;
      }
    }
  }
}
void set_elt(int elt[ELT_X][ELT_Y], int to) {
  int i, j;

  for(i=0; i<ELT_X; i++) {
    for(j=0; j<ELT_Y; j++) {
      elt[i][j] = to;
    }
  }
}

void print_elt(int elt[ELT_X][ELT_Y]) {
  int i, j;
  for(i=0; i<ELT_X; i++) {
    for(j=0; j<ELT_Y; j++) {
      printf("%d ", elt[i][j]);
    }
    printf("\n");
  }
}
