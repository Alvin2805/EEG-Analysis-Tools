#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void Manual(int argc);
void Musle();
void Xline();
void Yline();
void PutIn();
void Noise_Level();
void Gnuplot();


char fin[100], fout[100], fout_X[100], fout_Y[100], fout_N[100], file_name[100],file_name1[100],gnu[100] ,musle[100],titler[100];
FILE *FIN, *FOUT, *FGNU;
int i,j,eeg_ch,emg_ch ,point ,e,epoch;
double Freq, Coh, z ,range ,v ,x_max,y_max,noise_level,X,Y;

main(int argc, char *argv[ ])
{
    Manual(argc);
    sprintf(file_name,"%s",argv[1]);
    eeg_ch = atoi(argv[2]);
    emg_ch = atoi(argv[3]);
    range = atoi(argv[4]);
    point = atoi(argv[5]);
    x_max = atof(argv[6]);
    y_max = atof(argv[7]);
    epoch = atoi(argv[8]);
    X=7.0*x_max/6;
    Y=4.0*y_max/3;
    noise_level = 1 - pow(0.05,1.0/(epoch - 1));
    Musle();
    Xline();
    Yline();
    Noise_Level();
    PutIn();
    Gnuplot();
}    

void Manual(int argc)
{
    if(argc != 9)
	{
	    printf("\n 引数 \n");
	    printf("./multiplot <filename(.eeg_emg)> <last_eeg_ch> <emg_ch> <FFT_POINT> <POINT> <X_MAX> <Y_MAX> <EPOCH> \n \n");
	    exit(1);
        }
}

void Musle() // modified for murat
{
    if (emg_ch==19)
	{ 
          strcpy(musle,"EEG-EMG Coherence");
          strcpy(titler,file_name);
	}
    else if (emg_ch==6)
        {
        strcpy(musle,"EEG-EEG Coherence");
        strcpy(titler,file_name);
        } 
}
	
void Xline()
{       
    sprintf(fout_X,"%s_X_line",file_name); 
    FOUT = fopen(fout_X,"w");

    for(i=0;i<=eeg_ch;i++)
	{
	    for(j=0;j<=x_max;j++)
		if(i==0)
		    fprintf(FOUT,"%lf\t%lf\n", 10.0/6*x_max + j, 4*Y);
		else if(i==1)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0 + j, 3*Y);
		else if(i==2)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0 + j, 2*Y);
		else if(i==3)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0 + j, Y);
		else if(i==4)
		    fprintf(FOUT,"%lf\t%lf\n", 10.0/6*x_max + j, 0.0);
		else if(i==5)
		    fprintf(FOUT,"%lf\t%lf\n", X + j, 3*Y);
		else if(i==6)
		    fprintf(FOUT,"%lf\t%lf\n", X + j, 2*Y);
		else if(i==7)
		    fprintf(FOUT,"%lf\t%lf\n", X + j, Y);
		else if(i==8)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X + j, 3*Y);
		else if(i==9)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X + j, 2*Y);
		else if(i==10)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X + j, Y);
		else if(i==11)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X + j, 3*Y);
		else if(i==12)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X + j, 2*Y);
		else if(i==13)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X + j, Y);
		else if(i==14)
		    fprintf(FOUT,"%lf\t%lf\n", 18.0/6*x_max + j, 4*Y);
		else if(i==15)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X + j, 3*Y);
		else if(i==16)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X + j, 2*Y);
		else if(i==17)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X + j, Y);
		else if(i==18)
		    fprintf(FOUT,"%lf\t%lf\n", 18.0/6*x_max + j, 0.0);
             fprintf(FOUT,"\n");
	}
    fclose(FOUT);
}
 
void Yline()
{
    sprintf(fout_Y,"%s_Y_line",file_name); 
    FOUT = fopen(fout_Y,"w");
    
    for(i=0;i<=eeg_ch;i++)
	{
	    for(j=0;j<=2;j++)
		if(i==0)
		    fprintf(FOUT,"%lf\t%lf\n", 10.0/6*x_max , 4*Y+j*y_max/2);
		else if(i==1)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0, 3*Y+j*y_max/2);
		else if(i==2)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0, 2*Y+j*y_max/2);
		else if(i==3)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0, Y+j*y_max/2);
		else if(i==4)
		    fprintf(FOUT,"%lf\t%lf\n", 10.0/6*x_max, 0+j*y_max/2);
		else if(i==5)
		    fprintf(FOUT,"%lf\t%lf\n", X , 3*Y+j*y_max/2);
		else if(i==6)
		    fprintf(FOUT,"%lf\t%lf\n", X , 2*Y+j*y_max/2);
		else if(i==7)
		    fprintf(FOUT,"%lf\t%lf\n", X , Y+j*y_max/2);
		else if(i==8)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X, 3*Y+j*y_max/2);
		else if(i==9)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X, 2*Y+j*y_max/2);
		else if(i==10)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X, Y+j*y_max/2);
		else if(i==11)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X, 3*Y+j*y_max/2);
		else if(i==12)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X, 2*Y+j*y_max/2);
		else if(i==13)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X, Y+j*y_max/2);
		else if(i==14)
		    fprintf(FOUT,"%lf\t%lf\n", 18.0/6*x_max, 4*Y+j*y_max/2);
		else if(i==15)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X, 3*Y+j*y_max/2);
		else if(i==16)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X, 2*Y+j*y_max/2);
		else if(i==17)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X, Y+j*y_max/2);
		else if(i==18)
		    fprintf(FOUT,"%lf\t%lf\n", 18.0/6*x_max, 0+j*y_max/2);
             fprintf(FOUT,"\n");
	}
    fclose(FOUT);
}

void PutIn()
{
    sprintf(fout,"%s_newpoint",file_name);
    FOUT = fopen(fout,"w");
    z=1000/range;
    for(i=0;i<=eeg_ch;i++)
	{
	    sprintf(file_name1,"%s-1_ch%dch%d.linuxcsd",file_name,emg_ch,i);
	    j=0;
	    if((FIN=fopen(file_name1,"r"))==NULL)
		{
		    fprintf(stderr,"%sというファイルはありません．\n",file_name1);
		    exit(1);
		}
	    do
		{
		    if(emg_ch==19){ 
		    fscanf(FIN,"%lf\t%lf",&Freq, &Coh);
		    if(i==0)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 10.0/6*x_max, Coh + 4*Y);
		    else if(i==1)
			fprintf(FOUT,"%lf\t%lf\n", Freq, Coh + 3*Y);
		    else if(i==2)
			fprintf(FOUT,"%lf\t%lf\n", Freq, Coh + 2*Y);
		    else if(i==3)
			fprintf(FOUT,"%lf\t%lf\n", Freq, Coh + Y);
		    else if(i==4)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 10.0/6*x_max, Coh);
		    else if(i==5)
			fprintf(FOUT,"%lf\t%lf\n", Freq + X, Coh + 3*Y);
                    else if(i==6)
		    fprintf(FOUT,"%lf\t%lf\n", Freq + X, Coh + 2*Y);
		    else if(i==7)
			fprintf(FOUT,"%lf\t%lf\n", Freq + X, Coh + Y);
		    else if(i==8)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 2*X, Coh + 3*Y);
		    else if(i==9)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 2*X, Coh + 2*Y);
		    else if(i==10)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 2*X, Coh + Y);
		    else if(i==11)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 3*X, Coh + 3*Y);
		    else if(i==12)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 3*X, Coh + 2*Y);
		    else if(i==13)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 3*X, Coh + Y);
		    else if(i==14)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 18.0/6*x_max, Coh + 4*Y);
		    else if(i==15)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 4*X, Coh + 3*Y);
		    else if(i==16)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 4*X, Coh + 2*Y);
		    else if(i==17)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 4*X, Coh + Y);
		    else if(i==18)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 18.0/6*x_max, Coh);
		    fscanf(FIN,"\n");
		    v = z*j;
		    e=v;
		    j++;
		  }
		 else if(emg_ch==6){ 
		    fscanf(FIN,"%lf\t%lf",&Freq, &Coh);
		    if(i==0)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 10.0/6*x_max, Coh + 4*Y);
		    else if(i==1)
			fprintf(FOUT,"%lf\t%lf\n", Freq, Coh + 3*Y);
		    else if(i==2)
			fprintf(FOUT,"%lf\t%lf\n", Freq, Coh + 2*Y);
		    else if(i==3)
			fprintf(FOUT,"%lf\t%lf\n", Freq, Coh + Y);
		    else if(i==4)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 10.0/6*x_max, Coh);
		    else if(i==5)
			fprintf(FOUT,"%lf\t%lf\n", Freq + X, Coh + 3*Y);
                    //else if(i==6)
		    //fprintf(FOUT,"%lf\t%lf\n", Freq + X, Coh + 2*Y);
		    else if(i==7)
			fprintf(FOUT,"%lf\t%lf\n", Freq + X, Coh + Y);
		    else if(i==8)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 2*X, Coh + 3*Y);
		    else if(i==9)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 2*X, Coh + 2*Y);
		    else if(i==10)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 2*X, Coh + Y);
		    else if(i==11)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 3*X, Coh + 3*Y);
		    else if(i==12)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 3*X, Coh + 2*Y);
		    else if(i==13)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 3*X, Coh + Y);
		    else if(i==14)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 18.0/6*x_max, Coh + 4*Y);
		    else if(i==15)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 4*X, Coh + 3*Y);
		    else if(i==16)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 4*X, Coh + 2*Y);
		    else if(i==17)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 4*X, Coh + Y);
		    else if(i==18)
			fprintf(FOUT,"%lf\t%lf\n", Freq + 18.0/6*x_max, Coh);
		    fscanf(FIN,"\n");
		    v = z*j;
		    e=v;
		    j++;
		  }		
}
	    while(e < x_max);
	    fprintf(FOUT,"\n");
	    fclose(FIN);
	}
    fclose(FOUT);
}
	

void Noise_Level()
{       
    sprintf(fout_N,"%s_Noise_line",file_name); 
    FOUT = fopen(fout_N,"w");

    for(i=0;i<=eeg_ch;i++)
	{
	    for(j=0;j<=x_max;j++)
		if(i==0)
		    fprintf(FOUT,"%lf\t%lf\n", 10.0/6*x_max + j, 4*Y+noise_level);
		else if(i==1)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0 + j, 3*Y+noise_level);
		else if(i==2)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0 + j, 2*Y+noise_level);
		else if(i==3)
		    fprintf(FOUT,"%lf\t%lf\n", 0.0 + j, Y+noise_level);
		else if(i==4)
		    fprintf(FOUT,"%lf\t%lf\n", 10.0/6*x_max + j, 0.0+noise_level);
		else if(i==5)
		    fprintf(FOUT,"%lf\t%lf\n", X + j, 3*Y+noise_level);
		else if(i==6)
		    fprintf(FOUT,"%lf\t%lf\n", X + j, 2*Y+noise_level);
		else if(i==7)
		    fprintf(FOUT,"%lf\t%lf\n", X + j, Y+noise_level);
		else if(i==8)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X + j, 3*Y+noise_level);
		else if(i==9)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X + j, 2*Y+noise_level);
		else if(i==10)
		    fprintf(FOUT,"%lf\t%lf\n", 2*X + j, Y+noise_level);
		else if(i==11)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X + j, 3*Y+noise_level);
		else if(i==12)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X + j, 2*Y+noise_level);
		else if(i==13)
		    fprintf(FOUT,"%lf\t%lf\n", 3*X + j, Y+noise_level);
		else if(i==14)
		    fprintf(FOUT,"%lf\t%lf\n", 18.0/6*x_max + j, 4*Y+noise_level);
		else if(i==15)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X + j, 3*Y+noise_level);
		else if(i==16)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X + j, 2*Y+noise_level);
		else if(i==17)
		    fprintf(FOUT,"%lf\t%lf\n", 4*X + j, Y+noise_level);
		else if(i==18)
		    fprintf(FOUT,"%lf\t%lf\n", 18.0/6*x_max + j, 0.0+noise_level);
             fprintf(FOUT,"\n");
	}
    fclose(FOUT);
}
    
void Gnuplot()
    { 
	 FGNU=fopen("gnuplot.gp","w");
	 fprintf(FGNU,"set term tgif\n");
	 fprintf(FGNU,"set nokey\n\n");
	 fprintf(FGNU,"set output 'coherence_all_%s_%d_%d.obj'\n",file_name,emg_ch,point);
	 fprintf(FGNU,"set xrange [0:%lf]\n",5*X);
	 fprintf(FGNU,"set noxzeroaxis\n");
	 fprintf(FGNU,"set noxtics\n");
	 fprintf(FGNU,"set yrange [0:%lf]\n",5*Y);
	 fprintf(FGNU,"set noyzeroaxis\n");
	 fprintf(FGNU,"set noytics\n");
	 fprintf(FGNU,"set noborder\n");
	 //fprintf(FGNU,"set title '%s'\n",file_name);
	 fprintf(FGNU,"set title\n");
	 fprintf(FGNU,"set label 1 '%s' at 0,%f \n" ,titler,5*Y-y_max/3);
         fprintf(FGNU,"set label 2 '%s' at 0,%f \n" ,musle,5*Y-5.0*y_max/6);
	 fprintf(FGNU,"set label 3 '%.3f' at %f,%f \n" ,y_max,10.0/6*x_max-x_max/3,5*Y-y_max/3);
	 fprintf(FGNU,"set label 4 '%.0f' at %f,%f \n", x_max, 16.0/6*x_max-x_max/20, 4*Y-y_max/8);
	 fprintf(FGNU,"set label 5 '%.3f' at %f,%f \n" ,0.5*y_max,10.0/6*x_max-x_max/3,5*Y-5.0*y_max/6);
	 fprintf(FGNU,"set label 6 '%.0f' at %f,%f \n", 0.5*x_max,13.0/6*x_max-x_max/20,4*Y-y_max/8);
	 fprintf(FGNU,"set label 7 '%d' at %f,%f \n" ,0,10.0/6*x_max-x_max/10,4*Y-y_max/10);
	 fprintf(FGNU,"plot '%s' u 1:2 w l ,\'%s' u 1:2 w l -1 ,\'%s' u 1:2 w l -1 ,\'%s' u 1:2 w l \n",fout, fout_X, fout_Y, fout_N);
	 fprintf(FGNU,"exit \n");
	 fclose(FGNU);
	
	 /******** Run gnuplot **********/
	 system("gnuplot gnuplot.gp");

    	 system("rm *_line \n"); 
	 system("rm *_newpoint \n");
    }
