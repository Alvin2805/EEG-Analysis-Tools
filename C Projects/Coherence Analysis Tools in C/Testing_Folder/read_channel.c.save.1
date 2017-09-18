#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "CSD.h"

void channel(char *new_z, int time)
{
	int i, j, f, data1, data2, ch_number;
	int expe_time, expe_time_check, out_volt, i_ch, i_point, start, and;
	char fin[100], fout[100], fname[100], f_ch[100];
	FILE *FIN, *FOUT;
	
	for (f=1;f<=FILESUU;f++)
	{
		sprintf(fname,"%s-%d",new_z,f);
		sprintf(fin,"%s.eeg_emg",fname);
		FIN=fopen(fin,"rb");
		if (FIN==NULL)
		{
			printf("Cannot found the file %s, cannot proceed to next process (Remove Bad Epoch)\n",fin);
			exit(-1);
		}
		
		data1=fgetc(FIN);
		data2=fgetc(FIN);
		expe_time=(data1 | data2 << 8);
		expe_time_check=EXP_TIME;
		if (expe_time != expe_time_check)
		{
			printf("Please check your experiment data !!!\n");\
			exit(-1);
		}
		printf("Experiment Time = %d\n",expe_time);
		data1=fgetc(FIN);
		data2=fgetc(FIN);
		out_volt=(data1 | data2 <<8);
		printf("Out Voltage : %d \n",out_volt);
		for (i_ch=0;i_ch<CHANNELSUU<i_ch++)
		{
			sprintf(fout,"%s_ch%d.bin",fname,i_ch);
			FOUT=fopen(fout,"wb");
		}
	}
}

