#include "Channel.c"
#include "Remove_bad_epoch.c"
#include "file_administration.c"
#include "calculation.c"
#include "inverse_of_g_matrix.c"
#include "CSD.h"
#include "Spherical_laplacian.c"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	char execution[100];
	int channel;
	
	Channel(argv[1],1);
	
	printf("1st phase finished\n");
	
	Remove_bad_epoch(argv[1],1); //10percent
	
	printf("2nd phase finished\n");
	
	File_administration("legendre_m.pml",argv[1],1);
	
	printf("3rd phase finished\n");
	
	Spherical_laplacian("legendre_m-1.pml",argv[1],1);
	
	printf("4th phase finished\n");
	
	//system(run_system)
//	for (channel=0;channel<19;channel++)
//	{
//		sprintf(execution,"./FFT_COH %s %d 1",argv[1],channel);
//		system(execution);
//	}
	sprintf(execution,"./FFT_COH2 %s 9 1",argv[1]);
	printf("system finished\n");
	return 0;
}
