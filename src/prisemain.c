#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "../lib/def.h"
#include "../lib/nrio.h"
#include "../lib/nrarith.h"
#include "../lib/nralloc.h"

#define ELT_X 3
#define ELT_Y 3
#define NB_IMAGES 965
int main(int argc, char const *argv[]) {

  //save_nivgris();
  //detect_mvt(40);
  mean_temp();
  return 0;
}

void prisemain() {
  long nrh,nrl,nch,ncl;
  byte **R, **R2;
  int i, j;

  R = LoadPGM_bmatrix("../res/Test/carreTrou.pgm",&nrl,&nrh,&ncl,&nch);
  R2 = bmatrix(nrl, nrh, ncl, nch);

  dilate(R, R2, nrl, nrh, ncl, nch);
  dilate(R2, R, nrl, nrh, ncl, nch);
  dilate(R, R2, nrl, nrh, ncl, nch);
  dilate(R2, R, nrl, nrh, ncl, nch);
  dilate(R, R2, nrl, nrh, ncl, nch);
  dilate(R2, R, nrl, nrh, ncl, nch);
  dilate(R, R2, nrl, nrh, ncl, nch);
  dilate(R2, R, nrl, nrh, ncl, nch);
  dilate(R, R2, nrl, nrh, ncl, nch);
  dilate(R2, R, nrl, nrh, ncl, nch);
  dilate(R, R2, nrl, nrh, ncl, nch);

  SavePGM_bmatrix(R2,nrl,nrh,ncl,nch,"./prisemain.pgm");
  free_bmatrix(R, nrl, nrh, ncl, nch);
  free_bmatrix(R2, nrl, nrh, ncl, nch);
}

//TODO: Cleanup, do with all the images
void detect_mvt(int seuil) {
  long nrl, nrh, ncl, nch;
  byte **res;
  byte **cur, **new;
  int i, j, k;

  char *img_prefix = "../res/Sequences/Fomd/pgm/fomd";
  char *save_prefix = "../res/results/detection/mvt/";
  char *img_suffix = ".pgm";
  char img_path[50];
  char prev_path[50];
  char save_path[50];

  //Charge dans curbin la première image seuillée

  for(i=2; i<100; i++) {

    sprintf(prev_path, "%s%.3d%s", img_prefix, i-1, img_suffix);
    sprintf(img_path, "%s%.3d%s", img_prefix, i, img_suffix);
    printf("L:%s\n", prev_path);
    printf("L:%s\n", img_path);



    cur = LoadPGM_bmatrix(prev_path, &nrl, &nrh, &ncl, &nch);
    new = LoadPGM_bmatrix(img_path, &nrl, &nrh, &ncl, &nch);
    res = bmatrix(nrl, nrh, ncl, nch);
    for(j=0; j<=nrh; j++) { //TODO: Seuillé
      for(k=0; k<=nch; k++){
        if(abs(cur[j][k] - new[j][k]) < seuil){
          res[j][k] = 0;
        } else {
          res[j][k] = 255;
        }
      }
    }
    sprintf(save_path, "%s%.3d%s", save_prefix, i, img_suffix);
    printf("S:%s\n", save_path);
    SavePGM_bmatrix(res, nrl, nrh, ncl, nch, save_path);
  }
}

/*void mean_temp() {
  int nb_of_images = 100;
  int i, j, k;
  long nrl, nrh, ncl, nch;
  rgb8 **cur = LoadPPM_rgb8matrix("../res/Sequences/Fomd/ppm/fomd001.ppm", &nrl, &nrh, &ncl, &nch);
  long sum_r[ncl][nch];
  long sum_g[ncl][nch];
  long sum_b[ncl][nch];
  //memset( sum_r, 0, ncl*nch*sizeof(int) );
  //memset( sum_g, 0, ncl*nch*sizeof(int) );
  //memset( sum_b, 0, ncl*nch*sizeof(int) );
  my_zero_matrix(sum_r, ncl, nch);
  my_zero_matrix(sum_g, ncl, nch);
  my_zero_matrix(sum_b, ncl, nch);
  rgb8 **mean = rgb8matrix(nrl, nrh, ncl, nch);
  char *img_prefix = "../res/Sequences/Fomd/ppm/fomd";
  char *img_suffix = ".ppm";
  char img_path[50];

  for(i=0; i<nb_of_images; i++) {
    sprintf(img_path, "%s%.3d%s", img_prefix, i+1, img_suffix);
    cur = LoadPPM_rgb8matrix(img_path, &nrl, &nrh, &ncl, &nch);
    for(j = 0; j<nrh; j++) {
      for(k = 0; k<nch; k++) {
        sum_r[j][k] += cur[j][k].r;
        sum_g[j][k] += cur[j][k].g;
        sum_b[j][k] += cur[j][k].b;
        //mean[j][k] += (byte)((float)cur[j][k] / (float) nb_of_images);
      }
    }
  }

  for(j = 0; j<nrh; j++) {
    for(k = 0; k<nch; k++) {
      mean[j][k].r = sum_r[j][k] / nb_of_images;
      mean[j][k].g = sum_g[j][k] / nb_of_images;
      mean[j][k].b = sum_b[j][k] / nb_of_images;
    }
  }
  SavePPM_rgb8matrix(mean, nrl, nrh, ncl, nch, "../res/results/detection/mean_temp.ppm");
  free_rgb8matrix(cur, nrl, nrh, ncl, nch);
}*/

void mean_temp() {

  int nb_of_images = NB_IMAGES;
  long nrl, nrh, ncl, nch;
  rgb8 **cur = LoadPPM_rgb8matrix("../res/Sequences/Fomd/ppm/fomd001.ppm", &nrl, &nrh, &ncl, &nch);
  int i, j, k;

  rgb8 **mean = rgb8matrix(nrl, nrh, ncl, nch);
  char *img_prefix = "../res/Sequences/Fomd/ppm/fomd";
  char *img_suffix = ".ppm";
  char img_path[50];

  long sum_r[nrh][nch];
  long sum_g[nrh][nch];
  long sum_b[nrh][nch];
  memset( sum_r, 0, ncl*nch*sizeof(int) );
  memset( sum_g, 0, ncl*nch*sizeof(int) );
  memset( sum_b, 0, ncl*nch*sizeof(int) );

  for(i=0; i<nb_of_images; i++) {

    sprintf(img_path, "%s%.3d%s", img_prefix, i+1, img_suffix);
    cur = LoadPPM_rgb8matrix(img_path, &nrl, &nrh, &ncl, &nch);


    for(j=0; j<nrh; j++) {
      for(k=0; k<nch; k++) {
        sum_r[j][k] += cur[j][k].r;
        sum_g[j][k] += cur[j][k].g;
        sum_b[j][k] += cur[j][k].b;
      }
    }
  }

  for(j=0; j<nrh; j++) {
    for(k=0; k<nch; k++) {
      mean[j][k].r = sum_r[j][k] / nb_of_images;
      mean[j][k].g = sum_g[j][k] / nb_of_images;
      mean[j][k].b = sum_b[j][k] / nb_of_images;
    }
  }
  SavePPM_rgb8matrix(mean, nrl, nrh, ncl, nch, "../res/results/detection/mean_temp.ppm");
}

void mean_med() {
  //TODO
}
//TODO: struct for structuring element
void erode(byte **from, byte **to, long nrl, long nrh, long ncl, long nch) {
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

void dilate(byte **from, byte **to, long nrl, long nrh, long ncl, long nch) {
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

void get_nivgris(rgb8 **from, byte **to, long nrl, long nrh, long ncl, long nch) {
  int i, j;
  for (i = 0 ; i < nrh ; i++) {
    for(j = 0 ; j < nch ; j++) {
      to[i][j] = from[i][j].r/3 + from[i][j].g/3 + from[i][j].b/3;
    }
  }
}

void save_nivgris() {
  long nrl, nrh, ncl, nch;
  int i;
  char *img_prefix = "../res/Sequences/Fomd/ppm/fomd";
  char *save_prefix = "../res/Sequences/Fomd/pgm/fomd";
  char *img_suffix = ".ppm";
  char *save_suffix = ".pgm";
  char save_path[50], img_path[50];
  rgb8 **cur;
  byte **grey;

  for(i=1; i<965; i++) {
    sprintf(img_path, "%s%.3d%s", img_prefix, i, img_suffix);
    sprintf(save_path, "%s%.3d%s", save_prefix, i, save_suffix);

    cur = LoadPPM_rgb8matrix(img_path, &nrl, &nrh, &ncl, &nch);
    grey = bmatrix(nrl, nrh, ncl, nch);
    get_nivgris(cur, grey, nrl, nrh, ncl, nch);
    SavePGM_bmatrix(grey, nrl, nrh, ncl, nch, save_path);
  }
}
void get_seuillee(rgb8 **from, byte **to, int seuil, long nrl, long nrh, long ncl, long nch) {
  int i, j;
  get_nivgris(from, to, nrl, nrh, ncl, nch);
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

void my_zero_matrix(long **matrix, long x, long y) {
  int i, j;
  for(i=0; i<x; i++) {
    for(j=0; j<y; j++) {
      matrix[i][j] = 0;
    }
  }
}
