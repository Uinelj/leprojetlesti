#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int argc, char const *argv[]) {
  int elt[3][3];
  set(elt, 1);
  int img[10][10];
  int dest[10][10];
  set_img(img);
  noise(img);
  set_img(dest);

  print_mat(img);
  dilate(elt, img, dest);
  print_mat(dest);
  erode(elt, dest, img);
  print_mat(img);
  return 0;
}

void set(int elt[3][3], int to) {
  int i, j;

  for(i=0; i<3; i++) {
    for(j=0; j<3; j++) {
      elt[i][j] = to;
    }
  }
}

//TODO boundchecks
void erode(int elt[3][3], int src[10][10], int dest[10][10]) {
  int i, j;

  for(i=0; i<10; i++) {
      for(j=0; j<10; j++) {
        if(src[i-1][j-1] == 0 || src[i][j-1] == 0 || src[i+1][j-1] == 0 ||
           src[i-1][j] == 0 || src[i][j] == 0 || src[i+1][j] == 0 ||
           src[i-1][j+1] == 0 || src[i][j+1] == 0 || src [i+1][j+1] == 0) { //Si un truc du voisinage est ==0
             dest[i][j] = 0;
           } else {
             dest[i][j] = 1;
           }
      }
  }
}

void dilate(int elt[3][3], int src[10][10], int dest[10][10]) {
  int i, j;

  for(i=1; i<10; i++) {
      for(j=1; j<10; j++) {
        if(src[i-1][j-1] == 1 || src[i][j-1] == 1 || src[i+1][j-1] == 1 ||
           src[i-1][j] == 1 || src[i][j] == 1 || src[i+1][j] == 1 ||
           src[i-1][j+1] == 1 || src[i][j+1] == 1 || src [i+1][j+1] == 1) { //Si un truc du voisinage est ==1
             dest[i][j] = 1;
           } else {
             dest[i][j] = 0;
           }
      }
  }
}

void set_img(int elt[10][10]) {
  int i, j;

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {
      elt[i][j] = 0;
    }
  }
}

void noise(int elt[10][10]) {
    elt[0][4] = 1;
    elt[1][4] = 1;
    elt[4][3] = 1;
    elt[3][5] = 1;
    elt[6][6] = 1;
    elt[9][3] = 1;
    elt[4][4] = 1;
    elt[3][7] = 1;
    elt[1][2] = 1;
}
void print_mat(int mat[10][10]) {
  int i, j;

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {
      printf("%d ", mat[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
