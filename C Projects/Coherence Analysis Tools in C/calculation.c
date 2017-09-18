/*############################################################################


              c行列の係数を導出するための関数


            void  Calculation(double **g,double *z);

	      
	戻り値  無し （c行列の係数値のアドレスをかえす)



############################################################################*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"CSD.h"



Calculation(double *z_mat ,double **inv_g_mat,double *c)
{
    
    
    
    int NUM,MAT,MAT2;
    
    double  *sum;                  /*方程式時での右辺数値*/
    

    char box[100];                  /*不必要文字列格納*/
    

    FILE *octave_f;
    FILE *out_file;



    /*####################  メモリ確保と初期化  ##########################*/
        
    
    sum = (double *)calloc(MATRIX+1,sizeof(double ));
    if(sum == NULL){
	puts("sum のメモリが確保できません。");
	exit(1);
    }
   
    
    /*####################################################################*/


  


     /*#########  方程式の右辺値の準備 ########*/

    for(MAT2=0;MAT2<=MATRIX;MAT2++){
      if(MAT2!=MATRIX)
	sum[MAT2] = z_mat[MAT2];
      
      else
	sum[MAT2] = 0.0;
    }
    
        
 
    /*###########  Cn の導出 #############################*/

    
    for(MAT2=0;MAT2<=MATRIX;MAT2++){
	c[MAT2] = 0.0;
	for(MAT=0;MAT<=MATRIX;MAT++){
	    c[MAT2] += inv_g_mat[MAT][MAT2]*sum[MAT];
	}
    }    
    


}
