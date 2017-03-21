#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "../lib/def.h"
#include "../lib/nrio.h"
#include "../lib/nrarith.h"
#include "../lib/nralloc.h"


void convolution(byte **src, byte **dest, float **filter, int filter_size_x, int filter_size_y,
                 long nrl, long nrh, long ncl, long nch);
/**
  * Detects movement in an image.
  *
  * \param seuil Seuil de détection
  * \param img_prefix Préfixe des images a charger (path + début du nom de fichier)
  * \param save_prefix Dossier de sauvegarde des images
  **/
void detect_mvt(int seuil, char *img_prefix, char *save_prefix);

/**
  * Detects movement in an image using a reference image
  *
  * \param seuil Seuil de détection
  * \param img_prefix Préfixe des images a charger (path + début du nom de fichier)
  * \param save_prefix Dossier de sauvegarde des images
  **/
void detect_mvt_ref(int seuil, int seuil_binarisation, int nb_images, char *img_prefix, char *save_prefix);

/**
  * Temporal mean.
  */
void mean_temp();

void mean_med();

//TODO: struct for structuring element
void erode(byte **from, byte **to, long nrl, long nrh, long ncl, long nch);
void dilate(byte **from, byte **to, long nrl, long nrh, long ncl, long nch);

void get_nivgris(rgb8 **from, byte **to, long nrl, long nrh, long ncl, long nch);

void save_nivgris();
void get_seuillee(rgb8 **from, byte **to, int seuil, long nrl, long nrh, long ncl, long nch);
