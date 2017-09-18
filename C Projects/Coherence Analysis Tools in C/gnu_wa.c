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
  double ymin, ymax;
  int xmin, xmax;
  
  xmin = atoi(argv[2]);
  xmax = atoi(argv[3]);
  ymin = atof(argv[4]);
  ymax = atof(argv[5]);

  if(argc==1)
    {
    printf("gnuplot���ȷ����������\n");
    printf("�ǡ�����1�������,y=1����\n");
    printf("����ˡ: gnu <�ǡ����ե�����̾> <x�Ǿ���> <x������> <y�Ǿ���> <y������> \n");
    exit(-1);
    }  
  if(argc==6)
  {

    /******** gnuplot�ե����������ե�������� ********/

    fpGpFile=fopen("result.gp","w");   /* �ե�����̾��result.gp */
    fprintf(fpGpFile,"set term tgif\n");
    fprintf(fpGpFile,"set nokey\n\n");
    fprintf(fpGpFile,"set output '%s-%d-%d.obj'\n",argv[1],xmin,xmax);
    fprintf(fpGpFile,"set xrange [%d:%d]\n",xmin,xmax);
    fprintf(fpGpFile,"set yrange [%lf:%lf]\n",ymin,ymax);
    fprintf(fpGpFile,"set title '%s'\n",argv[1]);
    fprintf(fpGpFile,"plot '%s' u 1 w l\n",argv[1]);  /* ���������ե�������ɤ߹��� */
    /*fprintf(fpGpFile,"pause -1\n");*/
    fprintf(fpGpFile,"exit\n");
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
  exit(-1);
}









