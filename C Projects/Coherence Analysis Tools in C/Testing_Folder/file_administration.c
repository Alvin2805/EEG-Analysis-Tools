/*############################################################################


              a,z行列の定数をファイル処理により代入
	      するための関数


     File_administration(char *g_fname,char *z_file,int time_band);

	      
	          戻り値  なし



#############################################################################*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"CSD.h"


void inverse_of_g_matrix( ); 
void calculation();

File_administration(char *g_fname,char *z_file,int time_band)
{

    

    int  NUM,NUM1,MAT,MAT2; 
    int  counter;
    int  finish;                  /*プログラム終了判定 (feof()の値)*/    
    
    double *z_matrix;
    double *Cn;
    double  **inverse_g_matrix;   /*方程式時でのg行列の逆行列の係数*/
       

    char box[200];
    char **frame;
    char o_fname[50]="";               /*### 文字列内の初期化  ####*/
    char z_fname[50]="";            /*### 文字列内の初期化  ####*/


    FILE *g_fin;
    FILE *z_fin;
    FILE *output_f;


   
    /*############## 動的メモリ確保 #########################*/

   
    z_matrix= (double *)calloc(MATRIX,sizeof(double  ));
    if(z_matrix== NULL){
	puts("z_matrixのメモリが確保できません。");
	exit(1);
    }   
   
    Cn = (double *)calloc(MATRIX+1,sizeof(double));
    if(Cn == NULL){
	puts("c のメモリが確保できません。");
	exit(1);
    }

	        
    frame = (char **)malloc(2*sizeof(int *));
    if(frame == NULL){
	puts("frame のメモリが確保できません。");
	exit(1);
    }
    
    for(NUM=0;NUM<2;NUM++){
	*(frame + NUM) = (char *)malloc(100*sizeof(char ));
	if(*(frame + NUM) == NULL){
	    puts("frame のメモリが確保できません。");
	    exit(1);
	}
    }

    inverse_g_matrix = (double **)calloc(MATRIX + 1,sizeof(double *));
    if(inverse_g_matrix == NULL){
	puts("coeffcient_of_c のメモリが確保できません。");
	exit(1);
    }
    for(MAT=0;MAT<=MATRIX;MAT++){
	*(inverse_g_matrix + MAT) = (double *)calloc(MATRIX+1,sizeof(double ));
	if(*(inverse_g_matrix + MAT) == NULL){
	    puts("coeffcient_of_c のメモリが確保できません。");
	    exit(1);
	}
    }
    

    /*###################################################################*/


    /*################# 出力ファイル作成 ###############################*/

    sprintf(o_fname,"%s-%d.csd",z_file,time_band);


    output_f = fopen(o_fname,"w");
    if(output_f == NULL){
	printf("%s ファイルがオープンできません。(file_administration.c)\n",o_fname);
	exit(1);
    }
    
    
    
    /*##################################################################*/


    sprintf(z_fname,"%s-%d.mul",z_file,time_band);

    z_fin = fopen(z_fname,"r");      /*###z 行列のファイルを開きます  #*/
    if(z_fin==NULL){
	printf("%s ファイルがオープンできません。(file_administration.c)",z_fname);
	exit(1);
    }
    

    for(NUM=0;NUM<2;NUM++)         /*#出力ファイル用のコメント文の取得#*/
	fgets(frame[NUM],66,z_fin);
    
    
    for(NUM=0;NUM<2;NUM++)         /*#出力ファイルにコメント文の挿入#*/
	fprintf(output_f,"%s",frame[NUM]);

    
    counter =0;

    /*####################################################################*/
    

    Inverse_of_g_matrix(inverse_g_matrix,g_fname);   /*## g 行列の逆行列の準備  ##*/


    

    while(feof(z_fin)==0){          /*############  出力ファイル作成用のループ開始  ###############*/
	//for(NUM=0;NUM<2;NUM++){


	for(MAT2=0;MAT2<MATRIX;MAT2++){
	    fscanf(z_fin,"%s ",box);
	    z_matrix[MAT2] = atof(box);
	}
	
	



	
	Calculation(z_matrix,inverse_g_matrix,Cn);     /*#### c行列の係数を取得 ###*/
	
	
	for(MAT2=0;MAT2<=MATRIX;MAT2++)
		//printf("%lf\n",Cn[MAT2]);
	    fprintf(output_f,"%lf ",Cn[MAT2]);
	

	
	fprintf(output_f,"\n");
	++counter;
	

	/*if(counter%10000==0){
	  printf("%d %lf %lf \n",counter,z_matrix[0],z_matrix[1] );
	  }*/
	
	
    
    }       /*####  出力ファイル作成用のループ終了 ######*/
    
    printf("%d回の試行で終了しました。\n",counter);
    fclose(z_fin);
    fclose(output_f);
}
     
