/*#############################################################################

             Function that change raw data from Binary mode to ASCII mode

                       Channel(char *new_z,int time)

                             No return value
###############################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include"CSD.h"


Channel(char *new_z,int time)
{
  int i, j, f, data1, data2, ch_number;

  int expe_time, out_volt, i_ch, i_point, start, end; 
   
  char fin[100], fname[100], fout[100], f_ch[100];

  FILE *FIN, *FOUT;

  system("rm *.ascii");    /*  For precaution : *.ascii file is an append mode file */

  for(f=1;f<=FILESUU;f++){
   
    /*################Open for Read Input(Raw Data) Files################*/
      
    sprintf(fname,"%s-%d",new_z,f);
    
    sprintf(fin,"%s.eeg_emg",fname);
  
    FIN = fopen(fin,"rb");
    if (FIN == NULL){
      printf("Cannot open file %s. (Remove_bad_epoch) \n",fin);
      exit(-1);
    }
    
    /*################Read Input(Raw) Data(Binary Mode) and Create Output Files(Binary Mode) according to Channels################*/
    
    data1=fgetc(FIN);
    data2=fgetc(FIN);
    expe_time=(data1 | data2 << 8);
    expe_time=EXP_TIME;
    printf("%d.Experiment_Time = %d\n",f,expe_time);
    data1=fgetc(FIN);
    data2=fgetc(FIN);
    out_volt=(data1 | data2 << 8);
    printf("Out_Voltage = %d\n",out_volt);
    
    
    for(i_ch=0;i_ch<CHANNELSUU;i_ch++){
      
      sprintf(fout,"%s_ch%d.bin",fname,i_ch);
      FOUT=fopen(fout,"wb");
      
      for(i_point=0;i_point<(expe_time - WANTEDDATA);i_point++){
	data1=fgetc(FIN);  data2=fgetc(FIN);
      }
      for(i_point=(expe_time - WANTEDDATA);i_point<(expe_time * time);i_point++){
	data1=fgetc(FIN);  data2=fgetc(FIN);
	fputc(data1,FOUT);  fputc(data2,FOUT);
      }
      /*for(i_point=(expe_time * time);i_point<(expe_time * 3);i_point++){
	  data1=fgetc(FIN); data2=fgetc(FIN);
	  }*/
      fclose(FOUT);
    }

    fclose(FIN);
    
    /*################Change Raw Data(Channel base) Binary Mode to ASCII Mode################*/  
    
    for(ch_number = 0; ch_number <CHANNELSUU; ch_number++){ 
      
      sprintf(f_ch, "%s_ch%d.bin",fname,ch_number);
      if((FIN=fopen(f_ch,"rb"))==NULL){
	fprintf(stderr, "I can't find %s. (Remove_bad_epoch) \n",f_ch);
	exit(-1);
      }   
      
      sprintf(fout,"%s_ch%d.ascii",new_z,ch_number);
      FOUT=fopen(fout,"a+");
      
      for(j=0;j<(EPOCH * INTERVAL);j++){
	data1=fgetc(FIN); data2=fgetc(FIN);
	if(ch_number == EMG_CH){
	    fprintf(FOUT,"%lf\n",((data1 | data2 << 8) - 32768) *2500 * 1.0/32768);
	}
	else{
	fprintf(FOUT,"%lf\n",((data1 | data2 << 8) - 32768) * 500 * 1.0/32768);
	}
      }
      fclose(FIN);
      fclose(FOUT);
    }
   
  }
  
  system("rm *.bin"); 
  
  
}
