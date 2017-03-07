#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"

int main(int argc, char const *argv[]) {
  int seuil = 127;
  //q1(seuil);
  q2();
  return 0;
}

void q1(int seuil){
  long nrh,nrl,nch,ncl;
  byte **R, **R2;
  int i, j;

  R = LoadPGM_bmatrix("../../ppm/rice.pgm",&nrl,&nrh,&ncl,&nch);
  R2 = bmatrix(nrl, nrh, ncl, nch);

  for (i = 1 ; i < nrh ; i++) {
    for(j = 1 ; j < nch ; j++) {
      //printf("%d\n", R[i][j]);
      if(R[i][j] > seuil) {
        R2[i][j] = 255;
      } else {
        R2[i][j] = 0;
      }
    }
  }

  SavePGM_bmatrix(R2,nrl,nrh,ncl,nch,"./q1.pgm");
  free_bmatrix(R, nrl, nrh, ncl, nch);
  free_bmatrix(R2, nrl, nrh, ncl, nch);
}

void q2() {
  long nrh,nrl,nch,ncl;
  byte **R, **R2;
  int tmp[nrh][nch];
  int i, j;
  int region_counter = 1;

  R = LoadPGM_bmatrix("./q1.pgm",&nrl,&nrh,&ncl,&nch);
  R2 = bmatrix(nrl, nrh, ncl, nch);

  for (i = 1 ; i < nrh ; i++) {
    for(j = 1 ; j < nch ; j++) {
      /*if(R[i][j] == 255) {
        if(R[i-1][j] == 255 || R[i][j-1] == 255) {
          R2[i][j] = region_counter;
        } else {
          region_counter++;
          R2[i][j] = region_counter; //optimiser
        }

      }*/

      if(R[i][j] == R[i-1][j] && R[i][j] != R[i][j-1]){

        R2[i][j] = R2[i-1][j];

      } else if(R[i][j] == R[i][j-1] && R[i][j] != R[i-1][j]){

        R2[i][j] = R2[i][j-1];

      } else if(R[i][j] != R[i][j-1] && R[i][j] != R[i-1][j]) {
        region_counter++;
        R2[i][j] = region_counter;
      } else if(R[i][j] == R[i][j-1] && R[i][j] == R[i-1][j] && R2[i][j-1] == R2[i-1][j]){
        R2[i][j] = R2[i][j-1];
      } else if(R[i][j] == R[i][j-1] && R[i][j] == R[i-1][j] && R2[i][j-1] != R2[i-1][j]) {
        R2[i][j] = R2[i][j-1];

        int x, y;
        for(x=1; x<i; x++) {
          for(y=1; y<nch; y++) {
            if(R2[x][y] == R2[i-1][j]) {
              R2[x][y] = R2[i][j-1];
            }
          }
        }
      }
    }
  }

  //normalization
  /*for (i = 1 ; i < nrh ; i++) {
    for(j = 1 ; j < nch ; j++) {
      R2[i][j] = (byte) ((float)R2[i][j]/(float)region_counter)*255.0;
    }
  }*/

  printf("[q2]: region_counter:%d\n", region_counter);
  SavePGM_bmatrix(R2,nrl,nrh,ncl,nch,"./q2.pgm");
  free_bmatrix(R, nrl, nrh, ncl, nch);
  free_bmatrix(R2, nrl, nrh, ncl, nch);
}
