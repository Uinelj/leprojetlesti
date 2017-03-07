#include<stdio.h>
#include<stdlib.h>

#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"



int main(void){

	long nrh,nrl,nch,ncl;
	rgb8 **I;
	byte **R;


	
	int i,j;
	
	I=LoadPPM_rgb8matrix("cubesx3.ppm",&nrl,&nrh,&ncl,&nch);
	
	LoadPGM_bmatrix
	
	R=bmatrix(nrl,nrh,ncl,nch);


	SavePPM_rgb8matrix(I,nrl,nrh,ncl,nch,"cubesx3_bis.ppm");
	
	free_rgb8matrix(I,nrl,nrh,ncl,nch);


	return 1;

}
