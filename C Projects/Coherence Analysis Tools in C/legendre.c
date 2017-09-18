/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
   Program : Calculate Legendre Polynomials of cosine(angle) between 2 scalp electrode and Gm  
   Year    : 17.6.2002
   Compile : gcc -o legendre legendre.c -lm
   To Run  : ./legendre 
   Copyright (C) 2002 by Norlaili M S 
   Produced (P) 2002 by Murayama Laboratory
   ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.141592654      

FILE *fpi1, *fpo;
char fileinp[200]="angle";  /* "angle" is inputfile name */
char file[200], file_h[200];
char string[256], electrode[20][50];
int i, j, k, n;
double s, t, np, pp, angle[100][100], p[100], aa[100], bb[100], gm[100][100];

void Manual( );
void FileCheck( );
void Read( );
void Legendre( );
void Write( );
void Legendre_H( );
void Write_H( );

main(int argc, char *argv[ ]){
    if (argc != 1) Manual(argc);
    sprintf(file, "legendre_m.pml");    /* Outputfile for m = m */
    sprintf(file_h,"legendre_m-1.pml");  /* Outputfile for m = m -1 */
    FileCheck( );
    Read( );
    Legendre( );
    Write( );
    Legendre_H( );
    Write_H( );
}

void Manual( ){
    printf("\n To run programme type : \n");
    printf("          ./legendre \n\n");
    exit(1);
}

void FileCheck( ){
    if ((fpi1 = fopen(fileinp, "r")) == NULL){
	printf("no file %s \n", fileinp);
	exit(-1);
    }
}
void Read( ){
    fpi1=fopen(fileinp, "r");
    fgets(string,256,fpi1);
    for(i=0;i<19;i++){
	fscanf(fpi1, "%s ", electrode[i]);
	for(j=0;j<19;j++){
	    fscanf(fpi1, "%lf ", &angle[i][j]); 
	}
    }
    fclose(fpi1);
}

void Legendre( ){
    p[0]=1;
    for(j=0;j<19;j++){
	for(i=0;i<19;i++){
	    p[1]=angle[i][j];
	    for(k=2;k<8;k++){
		aa[k]= ((2*k)-1) * angle[i][j] * p[k-1];
		bb[k]= (k-1) * p[k-2];
		p[k]= (aa[k] - bb[k])/k ;    /* Legendre Polynomial */
	    }
	    pp = 0;
	    for(n=1;n<8;n++){
		s = pow(n*(n+1),4);
		t = (2*n) + 1;
		np = (t/s) * p[n];
		pp+=np;
	    }
	    gm[i][j] = (1/(4*pi)) * pp;
	}
    }
}

void Write( ){
    fpo=fopen(file,"w");
    fprintf(fpo, " These are the value of function Gm(cos(theta)). \n");
    fprintf(fpo," Fp1 F7 T3 T5 O1 F3 C3 P3 Fz Cz Pz F4 C4 P4 Fp2 F8 T4 T6 O2 \n");
    for(i=0;i<19;i++){
	fprintf(fpo, "%s ", electrode[i]);
	for(j=0;j<19;j++){
	    fprintf(fpo, "%lf ", gm[i][j]);
	}
	fprintf(fpo, "\n");
    }
    fclose(fpo);
}

void Legendre_H( ){
    p[0]=1;
    for(j=0;j<19;j++){
	for(i=0;i<19;i++){
	    p[1]=angle[i][j];
	    for(k=2;k<21;k++){
		aa[k]= ((2*k)-1) * angle[i][j] * p[k-1];
		bb[k]= (k-1) * p[k-2];
		p[k]= ( aa[k] - bb[k])/k ;    /* Legendre Polynomial */
	    }
	    pp = 0;
	    for(n=1;n<21;n++){
		s = pow(n*(n+1),3);
		t = (2*n) + 1;
		np = (t/s) * p[n];
		pp+=np;
	    }
	    gm[i][j] = (1/(4*pi)) * pp;
	}
    }
}

void Write_H( ){
    fpo=fopen(file_h,"w");
    fprintf(fpo, " These are the value of function G(m-1)(cos(theta)). \n");
    fprintf(fpo," Fp1 F7 T3 T5 O1 F3 C3 P3 Fz Cz Pz F4 C4 P4 Fp2 F8 T4 T6 O2 \n");
    for(i=0;i<19;i++){
	fprintf(fpo, "%s ", electrode[i]);
	for(j=0;j<19;j++){
	    fprintf(fpo, "%lf ", gm[i][j]);
	}
	fprintf(fpo, "\n");
    }
    fclose(fpo);
}
