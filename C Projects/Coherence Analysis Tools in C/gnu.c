/* gnuplot�ǿޤ�Ф��ץ���� */


/*
 * ���:�ǡ�����1�������,y=1����   
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000

main(int argc,char *argv[])
{
  FILE   *fpGpFile;
  double xmin, xmax, ymin, ymax, epoch,noise_level;
  
  xmin = atof(argv[2]);
  xmax = atof(argv[3]);
  ymin = atof(argv[4]);
  ymax = atof(argv[5]);
  epoch = atof(argv[6]);

  if(argc==1)
    {
    printf("gnuplot���ȷ����������\n");
    printf("�ǡ�����1�������,y=1����\n");
    printf("����ˡ: gnu <�ǡ����ե�����̾> <x�Ǿ���> <x������> <y�Ǿ���> <y������> <epoch>\n");
    exit(-1);
    }  
  if(argc==7)
  {
    /******** gnuplot�ե����������ե�������� ********/
    fpGpFile=fopen("result.gp","w");   /* �ե�����̾��result.gp */
    fprintf(fpGpFile,"set term tgif\n");
    fprintf(fpGpFile,"set nokey\n\n");
    fprintf(fpGpFile,"set output '%s.obj'\n",argv[1]);
    fprintf(fpGpFile,"set xrange [%lf:%lf]\n",xmin,xmax);
    fprintf(fpGpFile,"set yrange [%lf:%lf]\n",ymin,ymax);
    fprintf(fpGpFile,"set title '%s'\n",argv[1]);
    noise_level = 1 - pow(0.05,1/(epoch - 1));
    printf("Noise level is %f\n",noise_level);
    fprintf(fpGpFile,"plot '%s' u 1:2 w l,\%f w l\n",argv[1],noise_level);  /* ���������ե�������ɤ߹��� */
    fprintf(fpGpFile,"pause -1\n"); 
    fclose(fpGpFile);
    
    /******** gnuplot�����餻�� **********/
    system("gnuplot result.gp");
  
  }else
    {
    printf("gnuplot���ȷ����������\n");
    printf("�ǡ�����1�������,y=1����\n");
    printf("����ˡ: gnu <�ǡ����ե�����̾> <x�Ǿ���> <x������> <y�Ǿ���> <y������> <summation>\n");
    exit(-1);
    }
}









