/*############################################################################


              c����η�����Ƴ�Ф��뤿��δؿ�


            void  Calculation(double **g,double *z);

	      
	�����  ̵�� ��c����η����ͤΥ��ɥ쥹�򤫤���)



############################################################################*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"CSD.h"



Calculation(double *z_mat ,double **inv_g_mat,double *c)
{
    
    
    
    int NUM,MAT,MAT2;
    
    double  *sum;                  /*���������Ǥα��տ���*/
    

    char box[100];                  /*��ɬ��ʸ�����Ǽ*/
    

    FILE *octave_f;
    FILE *out_file;



    /*####################  ������ݤȽ����  ##########################*/
        
    
    sum = (double *)calloc(MATRIX+1,sizeof(double ));
    if(sum == NULL){
	puts("sum �Υ��꤬���ݤǤ��ޤ���");
	exit(1);
    }
   
    
    /*####################################################################*/


  


     /*#########  �������α����ͤν��� ########*/

    for(MAT2=0;MAT2<=MATRIX;MAT2++){
      if(MAT2!=MATRIX)
	sum[MAT2] = z_mat[MAT2];
      
      else
	sum[MAT2] = 0.0;
    }
    
        
 
    /*###########  Cn ��Ƴ�� #############################*/

    
    for(MAT2=0;MAT2<=MATRIX;MAT2++){
	c[MAT2] = 0.0;
	for(MAT=0;MAT<=MATRIX;MAT++){
	    c[MAT2] += inv_g_mat[MAT][MAT2]*sum[MAT];
	}
    }    
    


}
