#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fid,*fout;
	char electrode[25][25];
	double **g;
	int NUM,i,j,MAT;
	int MATRIX=19;
	char string[256];
	
	g = (double **)calloc(MATRIX,sizeof(double *));
	
	for(MAT=0;MAT<MATRIX;MAT++)
	{
		*(g + MAT) = (double *)calloc(MATRIX,sizeof(double));
	}
	
	fid=fopen("legendre_m-1.pml","r");
	for (NUM=0;NUM<2;NUM++)
	{
		fgets(string,256,fid);
	}
	
	for(i=0;i<MATRIX;i++)
	{
		fscanf(fid, "%s ", electrode[i]);
		for(j=0;j<MATRIX;j++)
		{
			fscanf(fid, "%lf ", &g[i][j]);
		}
	}
	fclose(fid);
	
	fout=fopen("pml.csv","w");
	
	for (i=0;i<MATRIX;i++)
	{
		for (j=0;j<MATRIX;j++)
		{
			fprintf(fout,"%f\n",g[i][j]);
		}
	}
	
	fclose(fout);
	return 0;
	
}
