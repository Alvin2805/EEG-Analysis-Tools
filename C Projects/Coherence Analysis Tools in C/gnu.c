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
  double xmin, xmax, ymin, ymax, epoch,noise_level;
  
  xmin = atof(argv[2]);
  xmax = atof(argv[3]);
  ymin = atof(argv[4]);
  ymax = atof(argv[5]);
  epoch = atof(argv[6]);

  if(argc==1)
    {
    printf("gnuplotで波形をだすぞよ\n");
    printf("データは1列で配列,y=1列目\n");
    printf("使用法: gnu <データファイル名> <x最小値> <x最大値> <y最小値> <y最大値> <epoch>\n");
    exit(-1);
    }  
  if(argc==7)
  {
    /******** gnuplotファイルの設定ファイルを作る ********/
    fpGpFile=fopen("result.gp","w");   /* ファイル名はresult.gp */
    fprintf(fpGpFile,"set term tgif\n");
    fprintf(fpGpFile,"set nokey\n\n");
    fprintf(fpGpFile,"set output '%s.obj'\n",argv[1]);
    fprintf(fpGpFile,"set xrange [%lf:%lf]\n",xmin,xmax);
    fprintf(fpGpFile,"set yrange [%lf:%lf]\n",ymin,ymax);
    fprintf(fpGpFile,"set title '%s'\n",argv[1]);
    noise_level = 1 - pow(0.05,1/(epoch - 1));
    printf("Noise level is %f\n",noise_level);
    fprintf(fpGpFile,"plot '%s' u 1:2 w l,\%f w l\n",argv[1],noise_level);  /* 描きたいファイルを読み込む */
    fprintf(fpGpFile,"pause -1\n"); 
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
}









