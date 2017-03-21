#include "lib.h"


void convolution(byte **src, byte **dest, float **filter, int filter_size_x, int filter_size_y,
                 long nrl, long nrh, long ncl, long nch) {
    int i, j;
    int filter_center_index = ((filter_size_x-1)/2);
    for(i=1; i<nrh; i++) {
      for(j=1; j<nch; j++) {

        dest[i][j] = src[i-1][j-1]*filter[filter_center_index-1][filter_center_index-1]
                    + src[i-1][j]*filter[filter_center_index-1][filter_center_index]
                    + src[i-1][j+1]*filter[filter_center_index-1][filter_center_index+1]
                    + src[i][j-1]*filter[filter_center_index][filter_center_index-1]
                    + src[i][j]*filter[filter_center_index][filter_center_index]
                    + src[i][j+1]*filter[filter_center_index-1][filter_center_index+1]
                    + src[i+1][j-1]*filter[filter_center_index+1][filter_center_index-1]
                    + src[i+1][j]*filter[filter_center_index+1][filter_center_index]
                    + src[i-1][j+1]*filter[filter_center_index-1][filter_center_index+1];
      }
    }

}
/**
  * Detects movement in an image.
  *
  * \param seuil Seuil de détection
  * \param img_prefix Préfixe des images a charger (path + début du nom de fichier)
  * \param save_prefix Dossier de sauvegarde des images
  **/
void detect_mvt(int seuil, char *img_prefix, char *save_prefix) {
  long nrl, nrh, ncl, nch;
  byte **res;
  byte **cur, **new;
  int i, j, k;

  //char *img_prefix = "../res/Sequences/Fomd/pgm/fomd";
  //char *save_prefix = "../res/results/detection/mvt/";
  char *img_suffix = ".pgm";
  char img_path[50];
  char prev_path[50];
  char save_path[50];

  for(i=2; i<100; i++) {

    sprintf(prev_path, "%s%.3d%s", img_prefix, i-1, img_suffix);
    sprintf(img_path, "%s%.3d%s", img_prefix, i, img_suffix);
    printf("[fn:detect_mvt] L:%s\n", prev_path);
    printf("[fn:detect_mvt] L:%s\n", img_path);



    cur = LoadPGM_bmatrix(prev_path, &nrl, &nrh, &ncl, &nch);
    new = LoadPGM_bmatrix(img_path, &nrl, &nrh, &ncl, &nch);
    res = bmatrix(nrl, nrh, ncl, nch);

    for(j=0; j<=nrh; j++) {
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

/**
  * Detects movement in an image using a reference image
  *
  * \param seuil Seuil de détection
  * \param img_prefix Préfixe des images a charger (path + début du nom de fichier)
  * \param save_prefix Dossier de sauvegarde des images
  **/
void detect_mvt_ref(int seuil, int seuil_binarisation, int nb_images, char *img_prefix, char *save_prefix) {

  long nrl, nrh, ncl, nch;

  int i, j, k;

  char *img_suffix = ".pgm";
  char img_path[50];
  char save_path[50];

  mean_temp();

  rgb8 **ref = LoadPPM_rgb8matrix("../res/results/detection/mean_temp.ppm", &nrl, &nrh, &ncl, &nch);
  byte **ref_grey = bmatrix(nrl, nrh, ncl, nch);
  byte **ref_bin = bmatrix(nrl, nrh, ncl, nch);
  byte **res = bmatrix(nrl, nrh, ncl, nch);
  byte **cur = bmatrix(nrl, nrh, ncl, nch);

  get_seuillee(ref, ref_bin, seuil_binarisation, nrl, nrh, ncl, nch);

  char *cur_path[50];

  for(i=1; i<nb_images; i++) {

    sprintf(cur_path, "%s%.3d%s", img_prefix, i, img_suffix);
    printf("L:%s\n", cur_path);
    cur = LoadPGM_bmatrix(cur_path, &nrl, &nrh, &ncl, &nch);

    for(j=0; j<nrh; j++) {
      for(k=0; k<nch; k++) {

        if(abs(cur[j][k] - ref_bin[j][k]) <= seuil) {
          res[j][k] = 255;
        } else {
          res[j][k] = 0;
        }

      }
    }
    sprintf(save_path, "%s%.3d%s", save_prefix, i, img_suffix);
    printf("S:%s\n", save_path);
    SavePGM_bmatrix(res, nrl, nrh, ncl, nch, save_path);
  }
}

/**
  * Temporal mean.
  */
void mean_temp(int nb_images) {


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

  for(i=0; i<nb_images; i++) {

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
      mean[j][k].r = sum_r[j][k] / nb_images;
      mean[j][k].g = sum_g[j][k] / nb_images;
      mean[j][k].b = sum_b[j][k] / nb_images;
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
