/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
   Program : Calculate cosine(angle) between 2 scalp electrode  
   Year    : 13.6.2002
   Compile : gcc -o cos cos.c -lm
   To Run  : ./cos <output filename>
   Copyright (C) 2002 by Norlaili M S 
   Produced (P) 2002 by Murayama Laboratory
   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *fpi1, *fpo;
char fileinp[50]="coordinate";
char file[50];
char string[200], electrode[20][20];
int i, j;
double phi[20], theta[20], x[20], y[20], z[20];

void Manual( );
void Read( );
void Cosine( );
void Write( );

main(int argc, char *argv[ ]){
    if (argc != 2) Manual( );
    sprintf(file, "%s.cos", argv[1]);
    Read( );
    Cosine( );
    Write( );
    fclose(fpo);
}

void Manual( ){
    printf("\n Need to declare name of output file ! ! \n");
    printf("          ./cos <output filename> \n\n");
    exit(1);
}

void Read( ){
    fpi1=fopen(fileinp, "r");
    fgets(string,200,fpi1);
    fgets(string,200,fpi1);
    for(i=0;i<19;i++){
	fscanf(fpi1, "%s", &electrode[i][20]);
	fscanf(fpi1, "%lf", &phi[i]);
	fscanf(fpi1, "%lf", &theta[i]);
	fscanf(fpi1, "%lf", &x[i]);
	fscanf(fpi1, "%lf", &y[i]);
	fscanf(fpi1, "%lf", &z[i]);
    }
    fclose(fpi1);
}

void Cosine( ){
    for(j=0;j<19;j++){
	for(i=0;i<19;i++){
	    x[j][i] = (x[j]-x[i])*(x[j]-x[i]);
	    y[j][i] = (y[j]-y[i])*(y[j]-y[i]);
	    z[j][i] = (z[j]-z[i])*(z[j]-z[i]);
	    cosine[j][i] = 1 - ((x[j][i]+y[j][i]+z[j][i])/2);
	}
    }
}

void Write( ){
    fpo=fopen(file,"w");
    fprintf(fpo,"   Cz C4 F4 Fz F3 C3 P3 Pz P4 T4 F8 Fp2 Fp1 F7 T3 T5 O1 O2 T6\n");
    for(i=0;i<19;i++){
	fprintf(fpo, "%s", electrode[i][20]);
	for(j=0;j<19;j++){
	    fprintf(fpo, "%lf", cosine[j][i]);
	}
	fprintf(fpo, "\n");
    }
    fclose(fpo);
}










