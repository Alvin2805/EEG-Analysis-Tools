/*#############################################################################

                           Function for WA

                       WA_EEG(int argc, char *argv[ ])

                             No return value
###############################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include"CSD.h"

void Manual(int argc);

int MAT, i, j, total_data, total_electrodes, start, end, time, EEG_CH;

int i_data;

double sum, ave;

double **data;

double *xr;

char string[256], electrode[20], timepoints[20];

char fin[100], fin_py[100];

FILE *FIN, *FIN_PY;

main(int argc, char *argv[ ]){
    
  /*################Memory Allocation and Initialization################*/
    
    xr = (double *)calloc(INTERVAL,sizeof(double));
    if(xr == NULL){
	puts("xr のメモリが確保できません。");
	exit(1);
    }

    data = (double **)calloc(FILESUU*EPOCH*INTERVAL,sizeof(double *));
    if(data == NULL){
	puts("data のメモリが確保できません。");
	exit(1);
    }
    for(MAT=0;MAT<(FILESUU*EPOCH*INTERVAL);MAT++){
	*(data + MAT) = (double *)calloc(CHANNELSUU+1,sizeof(double));
	if(*(data + MAT) == NULL){
	    puts("data のメモリが確保できません。");
	    exit(1);
	}
    } 
  
    /*################Give EEGs Data Memory Space################*/
    
    Manual(argc);
    
    EEG_CH = atoi(argv[2]);
    time = atoi(argv[3]);
    sprintf(fin,"%s-%d.allcsd",argv[1],time);
    
    /* ファイルの存在を確認 */
    if((FIN=fopen(fin,"r"))==NULL){
	fprintf(stderr,"%sというファイルはありません．\n",fin);
	exit(1);
    }
    
    fscanf(FIN,"%s %d %s %d \n",timepoints, &total_data, electrode, &total_electrodes);
    fgets(string,256,FIN);
    for(j=0;j<total_data;j++){
	for(i=EEG_START;i<=EEG_END;i++){             
	    fscanf(FIN,"%lf",&data[j][i]);
	}
    }
    fclose(FIN);


/*################Save data################*/
    
    sprintf(fin_py,"%s-%d_ch%d.wa",argv[1],time,EEG_CH);
    
    FIN_PY=fopen(fin_py,"w");

    for(i=0;i<(total_data / INTERVAL);i++){
      
      start = i * INTERVAL;
      end   = (i+1) * INTERVAL;
      
      sum =0.0;

      for(j=start;j<end;j++){
          sum+=data[j][EEG_CH];
      }
      ave = sum/INTERVAL;

      i_data = 0;
      for(j=start;j<end;j++){
	  /*xr[i_data] = data[j][EEG_CH] - ave;*/
	  xr[i_data] = data[j][EEG_CH];
	  fprintf(FIN_PY,"%lf\n",xr[i_data]);
	  i_data++;
      }
    
    }
    fclose(FIN_PY);
      
}

void Manual(int argc){
    if(argc != 4){
        printf("To run programme type  \n");
        printf("                        ./wa_eeg <filename(-#.allcsd)> <EEG_CH> <time>\n\n");
        exit(1);
    }
}










