/* to change the power file into logartihmic scale */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000

main(int argc,char *argv[])
{
  FILE   *powfile;
  int i;
  double xmin, xmax, ymin, ymax, epoch;
  double pow_arr[5000],fre_arr[5000];   
  char fin[100], fout[100];

  FILE *FIN, *FOUT;

  
  xmin = atof(argv[2]);
  xmax = atof(argv[3]);
  ymin = atof(argv[4]);
  ymax = atof(argv[5]);

  if(argc==1)/*if the input is not valid*/
    {
    printf("Invalid Input!\n");
    printf("Please type in the following form:\n");
    printf("power_log <filename>\n");
    exit(-1);
    }  
  if(argc==6)/*if the input is valid*/
    {
     sprintf(fin,"%s",argv[1]);
     if((FIN=fopen(fin,"r"))==NULL){
       printf("Cannot open file.\n");
       perror(fin);
       exit(errno);
     }
     i=0;
     while(fscanf(FIN,"%lf\t%lf",&fre_arr[i],&pow_arr[i])!=EOF)
     {
       i++;
     } 
     /*file open*/
     fout=
     FOUT=
     k=0;
     for(k=0;k<i+1;k++)
     {
       fprintf(FOUT,"%lf\t%lf\n",&fre_arr[i],&pow_arr[i]);
     }
     /*file close*//
     


    
     
    }
   else
    {
    printf("error\n");
    exit(-1);
    }
}









