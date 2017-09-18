/* gnuplotで図を出すプログラム */


/*
 * 注意:データは1列で配列,y=1列目   
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
    printf("gnuplotで波形をだすぞよ\n");
    printf("データは1列で配列,y=1列目\n");
    printf("使用法: gnu <データファイル名> <x最小値> <x最大値> <y最小値> <y最大値> \n");
    exit(-1);
    }  
  if(argc==6)
  {

    /******** gnuplotファイルの設定ファイルを作る ********/

    fpGpFile=fopen("result.gp","w");   /* ファイル名はresult.gp */
    fprintf(fpGpFile,"set term tgif\n");
    fprintf(fpGpFile,"set nokey\n\n");
    fprintf(fpGpFile,"set output '%s-%d-%d.obj'\n",argv[1],xmin,xmax);
    fprintf(fpGpFile,"set xrange [%d:%d]\n",xmin,xmax);
    fprintf(fpGpFile,"set yrange [%lf:%lf]\n",ymin,ymax);
    fprintf(fpGpFile,"set title '%s'\n",argv[1]);
    fprintf(fpGpFile,"plot '%s' u 1 w l\n",argv[1]);  /* 描きたいファイルを読み込む */
    /*fprintf(fpGpFile,"pause -1\n");*/
    fprintf(fpGpFile,"exit\n");
    fclose(fpGpFile);
    
    /******** gnuplotを走らせる **********/
    system("gnuplot result.gp");
  
  }else
    {
    printf("gnuplotで波形をだすぞよ\n");
    printf("データは1列で配列,y=1列目\n");
    printf("使用法: gnu <データファイル名> <x最小値> <x最大値> <y最小値> <y最大値> <summation>\n");
    exit(-1);
    }
  exit(-1);
}









