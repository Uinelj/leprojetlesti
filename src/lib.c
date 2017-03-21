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

void mean_temp() {
  int nb_of_images = 100;
  int i, j, k;
  long nrl, nrh, ncl, nch;
  byte **cur = LoadPGM_bmatrix("../res/Sequences/Fomd/pgm/fomd001.pgm", &nrl, &nrh, &ncl, &nch);
  long sum[ncl][nch];
  memset( sum, 0, ncl*nch*sizeof(int) );
  my_zero_matrix(sum, ncl, nch);
  byte **mean = bmatrix(nrl, nrh, ncl, nch);
  char *img_prefix = "../res/Sequences/Fomd/pgm/fomd";
  char *img_suffix = ".pgm";
  char img_path[50];

  for(i=0; i<nb_of_images; i++) {
    sprintf(img_path, "%s%.3d%s", img_prefix, i+1, img_suffix);
    cur = LoadPGM_bmatrix(img_path, &nrl, &nrh, &ncl, &nch);
    for(j = 0; j<nrh; j++) {
      for(k = 0; k<nch; k++) {
        sum[j][k] += cur[j][k];
        //mean[j][k] += (byte)((float)cur[j][k] / (float) nb_of_images);
      }
    }
  }
  SavePGM_bmatrix(mean, nrl, nrh, ncl, nch, "../res/results/detection/mean_temp.pgm");
  free_bmatrix(cur, nrl, nrh, ncl, nch);
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

void my_zero_matrix(int **matrix, int x, int y) {
  int i, j;
  for(i=0; i<x; i++) {
    for(j=0; j<y; j++) {
      matrix[i][j] = 0;
    }
  }
}
