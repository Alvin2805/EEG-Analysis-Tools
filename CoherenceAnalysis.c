/*#############################################################################

             Library for calculating coherence between two time series signals
			 This program recently used for calculating coherence between
			 EEG and EMG signal in Tibialis Anterior Muscle Investigation
			 Re-built and Re-optimized by Alvin Sahroni (nomad2805@gmail.com)
			 
###############################################################################*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define  MATRIX             19
#define  PAI2               6.28318530717958648
#define  FILESUU            5
#define  CHANNELSUU         32
#define  EPOCH             	58
#define  INTERVAL         	1024 
#define  SAMPLING_FRE     	1000
#define  EMG_CH             19
#define  EEG_START          0
#define  EEG_END            18
#define  MAX_VOLT          	100
#define  EXP_TIME       	60000
#define  WANTEDDATA    	 	60000


void Channel(char *new_z,int time)
{
	int i, j, f, data1, data2, ch_number;
	int expe_time, out_volt, i_ch, i_point, start, end; 
	char fin[100], fname[100], fout[100], f_ch[100];
	FILE *FIN, *FOUT;

	//system("rm *.ascii");    /*  For precaution : *.ascii file is an append mode file */
	system("rm *.csv");
	
	for(f=1;f<=FILESUU;f++)
	{
    /*################Open for Read Input(Raw Data) Files################*/
      
		sprintf(fname,"%s-%d",new_z,f);
		sprintf(fin,"%s.eeg_emg",fname);
		FIN = fopen(fin,"rb");
		if (FIN == NULL)
		{
			printf("Cannot open file %s. (Remove_bad_epoch) \n",fin);
			exit(-1);
		}
    
		/*################Read Input(Raw) Data(Binary Mode) and Create Output Files(Binary Mode) according to Channels################*/
    
		data1=fgetc(FIN);
		data2=fgetc(FIN);
		expe_time=(data1 | data2 << 8);
		expe_time=EXP_TIME;
		printf("%d.Experiment_Time = %d\n",f,expe_time);
		data1=fgetc(FIN);
		data2=fgetc(FIN);
		out_volt=(data1 | data2 << 8);
		printf("Out_Voltage = %d\n",out_volt);
    
		for(i_ch=0;i_ch<CHANNELSUU;i_ch++)
		{
			sprintf(fout,"%s_ch%d.bin",fname,i_ch);
			FOUT=fopen(fout,"wb");
			for(i_point=0;i_point<(expe_time - WANTEDDATA);i_point++)
			{
				data1=fgetc(FIN);  data2=fgetc(FIN);
			}
		
			for(i_point=(expe_time - WANTEDDATA);i_point<(expe_time * time);i_point++)
			{
				data1=fgetc(FIN);  data2=fgetc(FIN);
				fputc(data1,FOUT);  fputc(data2,FOUT);
			}
			fclose(FOUT);
		}

		fclose(FIN);
    
		/*################Change Raw Data(Channel base) Binary Mode to ASCII Mode################*/  
    
		for(ch_number = 0; ch_number <CHANNELSUU; ch_number++)
		{ 
			sprintf(f_ch, "%s_ch%d.bin",fname,ch_number);
			if((FIN=fopen(f_ch,"rb"))==NULL){
			fprintf(stderr, "I can't find %s. (Remove_bad_epoch) \n",f_ch);
			exit(-1);
		}   
      
		sprintf(fout,"%s_ch%d.ascii",new_z,ch_number);
		FOUT=fopen(fout,"a+");
      
		for(j=0;j<(EPOCH * INTERVAL);j++)
		{
			data1=fgetc(FIN); data2=fgetc(FIN);
			if(ch_number == EMG_CH)
			{
				fprintf(FOUT,"%lf\n",((data1 | data2 << 8) - 32768) *2500 * 1.0/32768);
			}
			else
			{
				fprintf(FOUT,"%lf\n",((data1 | data2 << 8) - 32768) * 500 * 1.0/32768);
			}
		}
		fclose(FIN);
		fclose(FOUT);
		}
   
	}
	system("rm *.bin"); 
}

void Remove_bad_epoch(char *new_z,int time)
{
	int i, j, k, m, ch_number;
	int start, end, MAT; 
	double *eeg_volt;
	double **data;
	double data_emg, data_eeg;
	double Max_data;
	char fin[100], fout[100], fin_emg[100], fout_emg[100];
	FILE *FIN, *FOUT, *FIN_EMG, *FOUT_EMG;


	/*################Memory Allocation + Initialization############*/

	eeg_volt = (double *)calloc(FILESUU*EPOCH,sizeof(double));
	if(eeg_volt == NULL){
		puts("eeg_volt : memory allocation failed\n");
		exit(1);
	}

	data = (double **)calloc(FILESUU*EPOCH*INTERVAL,sizeof(double *));
	if(data == NULL){
		puts("data : memory allocation failed\n");
		exit(1);
	}
	
	for(MAT=0;MAT<(FILESUU*EPOCH*INTERVAL);MAT++){
		*(data + MAT) = (double *)calloc(CHANNELSUU+1,sizeof(double));
		if(*(data + MAT) == NULL){
			puts("data : memory allocation failed\n");
			exit(1);
		}
	}


  
	/*################Give Data Memory Allocation################*/

     
	for(ch_number = 0; ch_number < CHANNELSUU; ch_number++){ 
		sprintf(fin,"%s_ch%d.ascii",new_z,ch_number);
		if((FIN=fopen(fin,"r"))==NULL){
			perror(fin);
			exit(errno);
		} 
    
		for(j=0;j<(FILESUU * EPOCH * INTERVAL);j++){
			fscanf(FIN,"%lf",&data[j][ch_number]);
		}
		fclose(FIN);
	}

  
 
	/*################New File For EMG After Removed Bad Epochs################*/

	sprintf(fout_emg,"%s-%d_ch%d.0ch-csd",new_z,time,EMG_CH);
	FOUT_EMG=fopen(fout_emg,"w"); 

	/*################Calculate for Max Value( Max > MAXIMUM_VOLT ??)################*/


	k = 0;
	FILE *cek_epoch;
	cek_epoch=fopen("cek_ep.csv","w");
	for(ch_number = EEG_START; ch_number <= EEG_END; ch_number++){

		for(i=0;i<(FILESUU * EPOCH);i++){
      
			start = i * INTERVAL;
			end   = (i + 1) * INTERVAL;
           
			for(j=start;j<end;j++){
				Max_data = fabs(data[j][ch_number]);
				if (eeg_volt[i] < Max_data)
					eeg_volt[i] = Max_data;
				}
			fprintf(cek_epoch,"%f\n",eeg_volt[i]);
		}
	}
	
	fclose(cek_epoch);

	for(i=0;i<(FILESUU * EPOCH);i++){
    
		start = i * INTERVAL;
		end   = (i + 1) * INTERVAL;
    
		if (eeg_volt[i] < MAX_VOLT){
			for(j=start;j<end;j++){
				fprintf(FOUT_EMG,"%lf \n",data[j][EMG_CH]);
			}
		}
		else{
			k++;
			printf("Epoch %d is removed in file %d. \n",i, (i / EPOCH) + 1);
		}
	}
  
	fclose(FOUT_EMG);

	/*################Save Data(EEGs) in Output File for Z_Matrix################*/

	sprintf(fout,"%s-%d.mul",new_z,time);
	FOUT = fopen(fout,"w");

	fprintf(FOUT,"TimePoints= %d Channels= %d \n", ((FILESUU * EPOCH * INTERVAL) - (k * INTERVAL)), EEG_END - EEG_START +1);
	fprintf(FOUT," Fp1 F7 T3 T5 O1 F3 C3 P3 Fz Cz Pz F4 C4 P4 Fp2 F8 T4 T6 O2 \n");

	for(i=0;i<(FILESUU * EPOCH);i++){

		start = i * INTERVAL;
		end   = (i + 1) * INTERVAL;

		for(j=start;j<end;j++){

			if (eeg_volt[i] < MAX_VOLT){
				for(m=EEG_START;m<=EEG_END;m++){
				fprintf(FOUT," %lf",data[j][m]);
				}
			fprintf(FOUT,"\n");
			}
		}
	}
	fclose(FOUT);

	printf("Total data is %d. \n",(FILESUU * EPOCH * INTERVAL) - (k * INTERVAL));
	system("rm *.ascii");
}

void Inverse_of_g_matrix(double **inv_g_mat,char *g_fna)
{
	int NUM,MAT,MAT2,MAT3;
	int counter;         
	int	counter1 ;       
	int	counter2 ;        
	double  **coeffcient_of_c;    
	double  **g_matrix;           
	double  *sum;                 
	char box[100];                  
	double kensa_d[MATRIX+1][MATRIX+1];
	FILE *octave_f;
	FILE *out_file;
	FILE *g_matrix_file;

	coeffcient_of_c = (double **)calloc(MATRIX + 1,sizeof(double *));
	if(coeffcient_of_c == NULL){
		puts("coeffcient_of_c : memory allocation failed\n");
		exit(1);
	}
	for(MAT=0;MAT<=MATRIX;MAT++){
		*(coeffcient_of_c + MAT) = (double *)calloc(MATRIX+1,sizeof(double ));
		if(*(coeffcient_of_c + MAT) == NULL){
			puts("coeffcient_of_c : memory allocation failed\n");
			exit(1);
		}
	}

	g_matrix = (double **)calloc(MATRIX,sizeof(double *));
	if(g_matrix == NULL){
		puts("g_matrix : memory allocation failed\n");
		exit(1);
	}
	for(MAT=0;MAT<MATRIX;MAT++){
		*(g_matrix + MAT) = (double *)calloc(MATRIX,sizeof(double ));
		if(*(g_matrix + MAT) == NULL){
			puts("g_matrix : memory allocation failed\n");
			exit(1);
		}
	}

	g_matrix_file=fopen(g_fna,"r");
	if(g_matrix_file == NULL){
		printf("%s --> file was not existed\n",g_fna);
		exit(1);
	}

	for(NUM=0;NUM<2;NUM++) fgets(box,80,g_matrix_file);
	
	for(MAT2=0;MAT2<MATRIX;MAT2++){
		fscanf(g_matrix_file,"%s",box);
		for(MAT=0;MAT<MATRIX;MAT++){
			fscanf(g_matrix_file,"%lf",&g_matrix[MAT][MAT2]);
		}
	}

	fclose(g_matrix_file);

	for(MAT2=0;MAT2<MATRIX;MAT2++){
		for(MAT=0;MAT<=MATRIX;MAT++){
		if(MAT==0) coeffcient_of_c[MAT][MAT2] = 1.0;
		else coeffcient_of_c[MAT][MAT2] = g_matrix[MAT-1][MAT2];
		}
	}

	for(MAT=0;MAT<=MATRIX;MAT++){
		if(MAT==0) coeffcient_of_c[MAT][MATRIX] = 0.0;
		else coeffcient_of_c[MAT][MATRIX] = 1.0;
	}

	octave_f = fopen("inverse_file","w");
	if(octave_f==NULL){
		puts("inversed_file --> file was not existed\n");
		exit(1);
	}

	fprintf(octave_f,"M=[");

	for(MAT2=0;MAT2<=MATRIX;MAT2++){
		for(MAT=0;MAT<=MATRIX;MAT++){
			fprintf(octave_f,"%lf ",coeffcient_of_c[MAT][MAT2]); 
			if(MAT!=MATRIX) fprintf(octave_f,",");
			else if(MAT==MATRIX){
				if(MAT2!=MATRIX)
					fprintf(octave_f,";");
			}
		}
	}


	fprintf(octave_f,"];\n");
	fprintf(octave_f,"a=inv(M);\n");
	fprintf(octave_f,"save F_inv_dat a;\n");
	fclose(octave_f);

	system("octave -q inverse_file ");

	octave_f = fopen("F_inv_dat","r");
	if(octave_f == NULL){
		puts("F_inv_dat --> file was not existed\n");
		exit(1);
	}

	for(NUM=0;NUM<5;NUM++){
		fgets(box,100,octave_f);
	}

	for(MAT2=0;MAT2<=MATRIX;MAT2++){
		for(MAT=0;MAT<=MATRIX;MAT++){
			fscanf(octave_f,"%lf",&inv_g_mat[MAT][MAT2]);
		}
	}
	fclose(octave_f);
}

void Calculation(double *z_mat ,double **inv_g_mat,double *c)
{
	int NUM,MAT,MAT2;
	double  *sum;                  
	char box[100];                  
	FILE *octave_f;
	FILE *out_file;

	sum = (double *)calloc(MATRIX+1,sizeof(double ));
	if(sum == NULL){
		puts("sum : memory allocation failed\n");
		exit(1);
	}

/*####################################################################*/

	for(MAT2=0;MAT2<=MATRIX;MAT2++){
		if(MAT2!=MATRIX) sum[MAT2] = z_mat[MAT2];
		else sum[MAT2] = 0.0;
	}

	for(MAT2=0;MAT2<=MATRIX;MAT2++){
		c[MAT2] = 0.0;
		for(MAT=0;MAT<=MATRIX;MAT++){
			c[MAT2] = c[MAT2] + inv_g_mat[MAT][MAT2]*sum[MAT];
		}
	}    
}

void File_administration(char *g_fname,char *z_file,int time_band)
{
	int  NUM,NUM1,MAT,MAT2; 
	int  counter;
	int  finish;                      
	double *z_matrix;
	double *Cn;
	double  **inverse_g_matrix;   
	char box[200];
	char **frame;
	char o_fname[50]="";               
	char z_fname[50]="";           
	FILE *g_fin;
	FILE *z_fin;
	FILE *output_f;

	z_matrix= (double *)calloc(MATRIX,sizeof(double));
	if(z_matrix== NULL){
		puts("z_matrix : memory allocation failed\n");
		exit(1);
	}   

	Cn = (double *)calloc(MATRIX+1,sizeof(double));
	if(Cn == NULL){
		puts("Cn : memory allocation failed\n");
		exit(1);
	}

	frame = (char **)malloc(2*sizeof(int *));
	if(frame == NULL){
		puts("frame : memory allocation failed\n");
		exit(1);
	}

	for(NUM=0;NUM<2;NUM++){
		*(frame + NUM) = (char *)malloc(100*sizeof(char ));
		if(*(frame + NUM) == NULL){
			puts("frame : memory allocation failed\n");
			exit(1);
		}
	}

	inverse_g_matrix = (double **)calloc(MATRIX + 1,sizeof(double *));
	if(inverse_g_matrix == NULL){
		puts("coeffcient_of_c : memory allocation failed\n");
		exit(1);
	}
	for(MAT=0;MAT<=MATRIX;MAT++){
		*(inverse_g_matrix + MAT) = (double *)calloc(MATRIX+1,sizeof(double ));
		if(*(inverse_g_matrix + MAT) == NULL){
			puts("coeffcient_of_c : memory allocation failed\n");
			exit(1);
		}
	}
	
	sprintf(o_fname,"%s-%d.csd",z_file,time_band);
	output_f = fopen(o_fname,"w");
	if(output_f == NULL){
		printf("%s file error\n",o_fname);
		exit(1);
	}

/*##################################################################*/

	sprintf(z_fname,"%s-%d.mul",z_file,time_band);
	z_fin = fopen(z_fname,"r");      
	if(z_fin==NULL){
		printf("%s file error\n",z_fname);
		exit(1);
	}

	for(NUM=0;NUM<2;NUM++) fgets(frame[NUM],66,z_fin);
	for(NUM=0;NUM<2;NUM++) fprintf(output_f,"%s",frame[NUM]);
	counter =0;

/*####################################################################*/

	Inverse_of_g_matrix(inverse_g_matrix,g_fname);   

	while(feof(z_fin)==0){        
		for(MAT2=0;MAT2<MATRIX;MAT2++){
			fscanf(z_fin,"%s ",box);
			z_matrix[MAT2] = atof(box);
		}

		Calculation(z_matrix,inverse_g_matrix,Cn);     
		
		for(MAT2=0;MAT2<=MATRIX;MAT2++) fprintf(output_f,"%lf ",Cn[MAT2]);
		fprintf(output_f,"\n");
		++counter;
	}   
	
	printf("%d count\n",counter);
	fclose(z_fin);
	fclose(output_f);
}

void Spherical_laplacian(char *g_fname,char *new_zfname, int time_band)
{
	int i, j, k, total_data, total_electrodes;
	int NUM, MAT;
	double **data;
	double **g; 
	double n;
	double *sum;
	char file_c[200], file_g[200];
	char file_out[200];
	char string[256], electrode[25][25], timepoints[20], elect[20];
	FILE *f_g, *f_c, *f_out;

/*################Memory Allocation and Initalization################*/

	data = (double **)calloc(FILESUU*EPOCH*INTERVAL,sizeof(double *));
	if(data == NULL){
		puts("data : memory allocation failed\n");
		exit(1);
	}
	for(MAT=0;MAT<(FILESUU*EPOCH*INTERVAL);MAT++){
		*(data + MAT) = (double *)calloc(MATRIX+1,sizeof(double));
		if(*(data + MAT) == NULL){
			puts("data : memory allocation failed\n");
			exit(1);
		}
	}

	g = (double **)calloc(MATRIX,sizeof(double *));
	if(g == NULL){
		puts("g : memory allocation failed\n");
		exit(1);
	}
	for(MAT=0;MAT<MATRIX;MAT++){
		*(g + MAT) = (double *)calloc(MATRIX,sizeof(double));
		if(*(g + MAT) == NULL){
			puts("g : memory allocation failed\n");
			exit(1);
		}
	}

	sum = (double *)calloc(FILESUU*EPOCH*INTERVAL,sizeof(double));
	if(sum == NULL){
		puts("sum : memory allocation failed\n");
		exit(1);
	}

/*################Open for Read G(m-1) and Cn Input Files################*/

	sprintf(file_g,"%s",g_fname);
	f_g = fopen(file_g, "r");
	if (f_g == NULL){
		printf("Cannot open file %s. (Spherical_laplacian) \n",file_g);
		exit(-1);
	}

	sprintf(file_c,"%s-%d.csd",new_zfname,time_band);    
	f_c = fopen(file_c, "r");
	if (f_c == NULL){
		printf("Cannot open file %s. (Spherical_laplacian) \n",file_c);
		exit(-1);
	}


/*################Read G(m-1) and Cn Input Files Data###############*/

	for(NUM=0;NUM<2;NUM++) fgets(string,256,f_g);
	for(i=0;i<MATRIX;i++){
		fscanf(f_g, "%s ", electrode[i]);
		for(j=0;j<MATRIX;j++){
			fscanf(f_g, "%lf ", &g[i][j]);
		}
	}
	fclose(f_g);

	fscanf(f_c,"%s %d %s %d \n",timepoints, &total_data, elect, &total_electrodes);
	fgets(string,256,f_c);

	for(i=0;i<total_data;i++){
		for(j=0;j<(MATRIX+1);j++){
			fscanf(f_c, "%lf ", &data[i][j-1]);
		}
	}
	fclose(f_c);

/*################Calculate scalp current density(Perrin et. al. (1989))################*/

	for(k=0;k<total_data;k++){
		for(i=0;i<MATRIX;i++){
			sum[i] = 0.0;
			for(j=0;j<MATRIX;j++){
				n = g[i][j] * data[k][j];
				sum[i]+=n;
			}
		}
		for(i=0;i<MATRIX;i++){
			data[k][i] = sum[i];
		}
	}

/*################Create Output File################*/

	sprintf(file_out,"%s-%d.allcsd",new_zfname,time_band); 
	f_out = fopen(file_out, "w");
	if (f_out == NULL){
		printf("Cannot open file %s. (Spherical_laplacian) \n",file_out);
		exit(-1);
	}

/*################Save Data in Output File################*/

	fprintf(f_out,"%s %d %s %d \n",timepoints, total_data, elect, total_electrodes);
	fprintf(f_out,"%s",string);
	for(i=0;i<total_data;i++){
		for(j=0;j<MATRIX;j++){
			fprintf(f_out, "%lf ", data[i][j]);
		}
		fprintf(f_out, "\n");
	}
	fclose(f_out);
}

void Manual();

void main(int *argc, char *argv[])
{

	int MAT, i, j, total_data, total_electrodes, start, end, time, EEG_CH;
	int i_data, n, m, ip, j_data, jw, k, np, p, pp;
	int i_x, i_x_max, i_f, i_p, i_f_max, i_p_max;
	double sum, ave, new_data, freq_order;
	double **data;
	double *xr, *xi, *xpower, xpower_max, *st;
	double temp, tr, ti, wr, wi;
	double *xrr, *xii, *fre, *yy, *xx, **y;
	double cross_r, cross_i, px, py, cohi;
	char string[256], electrode[20], timepoints[20];
	char fin[100], fout_p[100], fout_ri[100], fout[100], fin_1[100], fin_2[100], fin_px[100], fin_py[100], fin_xsp[100], fout_power[100];
	FILE *FIN, *FOUT_P, *FOUT_RI, *FOUT, *FIN_CH1, *FIN_CH2, *FIN_PX, *FIN_PY, *FIN_XSP, *FOUT_POWER;
	FILE *tempfile;
	
/*#########################Preprocessing##############################*/
	Channel(argv[1],1); 									// Okay (done)
	printf("1st phase finished\n");
	Remove_bad_epoch(argv[1],1); 							
	printf("2nd phase finished\n");
	File_administration("legendre_m.pml",argv[1],1);
	printf("3rd phase finished\n");
	Spherical_laplacian("legendre_m-1.pml",argv[1],1);
	printf("4th phase finished\n");
/*################Memory Allocation and Initialization################*/

    xr = (double *)calloc(INTERVAL,sizeof(double));
    if(xr == NULL){
		puts("xr : Failed allocating the memory\n");
		exit(1);
	}

    st = (double *)calloc((INTERVAL/4)+1,sizeof(double));
	if(st == NULL){
		puts("st : Failed allocating the memory\n");
    	exit(1);
    }

    xi = (double *)calloc(INTERVAL,sizeof(double));
    if(xi == NULL){
		puts("xi : Failed allocating the memory\n");
		exit(1);
    }

    xpower = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xpower == NULL){
		puts("xpower : failed allocating the memory\n");
		exit(1);
    }

    xrr = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xrr == NULL){
		puts("xrr : Failed allocating the memory\n");
		exit(1);
    }

    xii = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xii == NULL){
		puts("xii : Failed allocating the memory\n");
		exit(1);
    }

    fre = (double *)calloc(INTERVAL/2,sizeof(double));
    if(fre == NULL){
		puts("fre : Failed allocating the memory\n");
		exit(1);
    }

    yy = (double *)calloc(INTERVAL/2,sizeof(double));
    if(yy == NULL){
		puts("yy : Failed allocating the memory\n");
		exit(1);
    }

    xx = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xx == NULL){
		puts("xx : Failed allocating the memory\n");
		exit(1);
    }

    y = (double **)calloc(FILESUU*EPOCH,sizeof(double *));
    if(y == NULL){
		puts("y : Failed allocating the memory\n");
		exit(1);
    }
    for(MAT=0;MAT<(FILESUU*EPOCH);MAT++){
		*(y + MAT) = (double *)calloc(INTERVAL/2,sizeof(double));
		if(*(y + MAT) == NULL){
	    	puts("y : Failed allocating the memory\n");
	    	exit(1);
		}
    }

    data = (double **)calloc(FILESUU*EPOCH*INTERVAL,sizeof(double *));
    if(data == NULL){
		puts("data : Failed allocating the memory\n");
		exit(1);
    }
    for(MAT=0;MAT<(FILESUU*EPOCH*INTERVAL);MAT++){
		*(data + MAT) = (double *)calloc(CHANNELSUU+1,sizeof(double));
		if(*(data + MAT) == NULL){
			puts("data : Failed allocating the memory\n");
			exit(1);
		}
    }
    /*################Give EEGs Data Memory Space################*/

    Manual(argc);

    EEG_CH = atoi(argv[2]);
    time = atoi(argv[3]);
    sprintf(fin,"%s-%d.allcsd",argv[1],time);

    /* Preparing the Data from files */
    if((FIN=fopen(fin,"r"))==NULL){
		fprintf(stderr,"%s undefined\n",fin);
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


    /*################ Take absolute value of EMG data and
      rewrite data as (|data - average|)---- Ch1################*/

    sprintf(fin,"%s-%d_ch%d.0ch-csd",argv[1],time,EMG_CH);

    if((FIN=fopen(fin,"r"))==NULL){
		fprintf(stderr, "Sorry, cannot find %s! \n",fin);
		exit(1);
    }


    tempfile=fopen("sementara1.csv","w");
    for(i=0;i<(total_data / INTERVAL);i++){
		start = i * INTERVAL;
		end   = (i + 1) * INTERVAL;
		sum = 0;

		for(j=start;j<end;j++){
	    	fscanf(FIN,"%lf", &data[j][EMG_CH]);
	    	data[j][EMG_CH] = fabs(data[j][EMG_CH]);
	    	sum+=data[j][EMG_CH];
		}

		ave = sum/INTERVAL;

		for(j=start;j<end;j++){
	    	new_data=data[j][EMG_CH] - ave;
	    	data[j][EMG_CH] = new_data;
	    	fprintf(tempfile,"%f\n",data[j][EMG_CH]);
		}
    }

    fclose(tempfile);
    fclose(FIN);

    /*################FFT Calculation for EMG_CH################*/

    for(i=0;i<(total_data / INTERVAL);i++){
		start = i * INTERVAL;
		end   = (i+1) * INTERVAL;
      	i_data = 0;

      	for(j=start;j<end;j++){
			xr[i_data] = data[j][EMG_CH];
			i_data++;
      	}
      	n=i_data;

      	i_data=1;
      	while(( (int)pow(2,i_data) % n ) != 0){
			i_data++;
		}
		m=i_data;

		/* FFT Calculation begin */

		/* Set Sine Table */

		freq_order = PAI2 / n;
		st[0] = 0.0;

		for(i_data=1; i_data < n/4; i_data++){
			st[i_data] = sin(i_data * freq_order);
		}
		st[n/4] = 1.0;
		/**********************************/
		for(i_data=0; i_data < n; i_data++){ /* Hanning Window + Initialize*/
			xr[i_data] = xr[i_data] / 2.0 * ( 1.0  -  cos( PAI2 * ( (double)i_data / (double)(n - 1)) )  );
			xi[i_data] = 0.0;
		}

		j_data = n/2;   /* Bit Reversal */

		for (i_data=1; i_data <= n-3; i_data++){
			if (i_data< j_data){
				temp = xr[i_data];
				xr[i_data] = xr[j_data];
				xr[j_data] = temp;
				temp = xi[i_data];
				xi[i_data] = xi[j_data];
				xi[j_data] = temp;
			}
			k = n/2;
			while (k <= j_data){
				j_data -= k;
				k /= 2;
			}
			j_data += k;
		}

		p = 1;  /* Butterfly Algorithm */
		np = n;
		for (k=1; k <= m; k++){
			pp = p + p;
			np /= 2;
			for (j_data=0; j_data < p; j_data++){
				jw = j_data * np;
				if (jw <= n/4){
					wr = st[n/4 - jw];
					wi = st[jw];
				}
				else{
					wr = - st[jw - n/4];
					wi = st[n/2 - jw];
				}
				wi = - wi;
				for(i_data=j_data; i_data < n; i_data += pp){
					ip = i_data + p;
					tr = xr[ip] * wr - xi[ip] * wi;
					ti = xr[ip] * wi + xi[ip] * wr;
					xr[ip] = xr[i_data] - tr;
					xi[ip] = xi[i_data] - ti;
					xr[i_data] = xr[i_data] + tr;
					xi[i_data] = xi[i_data] + ti;
				}
			}
			p = pp;
		}
		/* Power Spectral Calculation */
		xpower_max = 0;
		for (i_data=0; i_data < n/2; i_data++){
			xr[i_data] =  xr[i_data] * sqrt(8/3) * 0.001;
			xi[i_data] =  xi[i_data] * sqrt(8/3) * 0.001;
			xpower[i_data] = xr[i_data] * xr[i_data] + xi[i_data] * xi[i_data];
			xpower_max += xpower[i_data];
		}

		sprintf(fout_p,"%s_ch%d_%d-%d.fftp",argv[1],EMG_CH,start,end);
		FOUT_P = fopen(fout_p, "w");
		sprintf(fout_ri,"%s_ch%d_%d-%d.fftri",argv[1],EMG_CH,start,end);
		FOUT_RI = fopen(fout_ri, "w");

		for(i_data=0;i_data < n/2; i_data++){
			fprintf( FOUT_P, "%lf\t%lf\n", ( 1.0 * SAMPLING_FRE / n ) * ( i_data + 1 ), xpower[i_data]);
			fprintf( FOUT_RI, "%lf\t%lf\t%lf\n", ( 1.0 * SAMPLING_FRE / n ) * ( i_data + 1 ), xr[i_data], xi[i_data] );
		}
		fclose(FOUT_P);
		fclose(FOUT_RI);
	}

	/*################FFT Calculation for EEG_CH################*/

    for(i=0;i<(total_data / INTERVAL);i++){
		start = i * INTERVAL;
		end   = (i+1) * INTERVAL;
		sum =0.0;
		tempfile=fopen("sementara.csv","w");

		for(j=start;j<end;j++){
			sum+=data[j][EEG_CH];
		}
		ave = sum/INTERVAL;

		i_data = 0;
		for(j=start;j<end;j++){
			xr[i_data] = data[j][EEG_CH] - ave;
			fprintf(tempfile,"%f\n",data[j][EEG_CH]);
			i_data++;
		}

		fclose(tempfile);
		n=i_data;
		i_data=1;

		while(( (int)pow(2,i_data) % n ) != 0){
			i_data++;
		}
		m=i_data;

		/* FFT Calculation begin */

		/* Set Sine Table */

      	freq_order = PAI2 / n;
		st[0] = 0.0;

		for(i_data=1; i_data < n/4; i_data++){
			st[i_data] = sin(i_data * freq_order);
		}

		st[n/4] = 1.0;
		/**********************************/

		for(i_data=0; i_data < n; i_data++){ /* Hanning Window + Initialize*/
			xr[i_data] = xr[i_data] / 2.0 * ( 1.0  -  cos( PAI2 * ( (double)i_data / (double)(n - 1)) )  );
			xi[i_data] = 0.0;
		}

		j_data = n/2;   /* Bit Reversal */

		for (i_data=1; i_data <= n-3; i_data++){
			if (i_data< j_data){
				temp = xr[i_data]; xr[i_data] = xr[j_data]; xr[j_data] = temp;
				temp = xi[i_data]; xi[i_data] = xi[j_data]; xi[j_data] = temp;
			}
			k = n/2;
			while (k <= j_data){
				j_data -= k;
				k /= 2;
			}
			j_data += k;
		}
		p = 1;  /* Butterfly Algorithm */
		np = n;

		for (k=1; k <= m; k++){
			pp = p + p;
			np /= 2;

			for (j_data=0; j_data < p; j_data++){
				jw = j_data * np;

				if (jw <= n/4){
					wr = st[n/4 - jw];
					wi = st[jw];
				}
				else{
					wr = - st[jw - n/4];
					wi = st[n/2 - jw];
				}

				wi = - wi;

				for(i_data=j_data; i_data < n; i_data += pp){
					ip = i_data + p;
					tr = xr[ip] * wr - xi[ip] * wi;
					ti = xr[ip] * wi + xi[ip] * wr;
					xr[ip] = xr[i_data] - tr;
					xi[ip] = xi[i_data] - ti;
					xr[i_data] = xr[i_data] + tr;
					xi[i_data] = xi[i_data] + ti;
				}
			}
			p = pp;
		}

		/* Power Spectral Calculation */

		xpower_max = 0;
		for (i_data=0; i_data < n/2; i_data++){
			xr[i_data] =  xr[i_data] * sqrt(8/3) * 0.001;
			xi[i_data] =  xi[i_data] * sqrt(8/3) * 0.001;
			xpower[i_data] = xr[i_data] * xr[i_data] + xi[i_data] * xi[i_data];
			xpower_max += xpower[i_data];
		}

		sprintf(fout_p,"%s_ch%d_%d-%d.fftp",argv[1],EEG_CH,start,end);
		FOUT_P = fopen(fout_p, "w");
		sprintf(fout_ri,"%s_ch%d_%d-%d.fftri",argv[1],EEG_CH,start,end);
		FOUT_RI = fopen(fout_ri, "w");

		for(i_data=0;i_data < n/2; i_data++){
			fprintf( FOUT_P, "%lf\t%lf\n", ( 1.0 * SAMPLING_FRE / n ) * ( i_data + 1 ), xpower[i_data]);
			fprintf( FOUT_RI, "%lf\t%lf\t%lf\n", ( 1.0 * SAMPLING_FRE / n ) * ( i_data + 1 ), xr[i_data], xi[i_data] );
		}

		fclose(FOUT_P);
		fclose(FOUT_RI);
	}



    /*################Cross Spectral Calculation################*/

    for(i=0;i<(total_data / INTERVAL);i++){

		start = i * INTERVAL;
		end   = (i+1) * INTERVAL;

		sprintf(fin_1,"%s_ch%d_%d-%d.fftri",argv[1],EMG_CH,start,end);
		sprintf(fin_2,"%s_ch%d_%d-%d.fftri",argv[1],EEG_CH,start,end);

		if((FIN_CH1=fopen(fin_1,"r"))==NULL || (FIN_CH2=fopen(fin_2,"r"))==NULL){
			fprintf(stderr, "I can't find %s and %s. I'm sorry !! \n",fin_1,fin_2);
			exit(-1);
		}

		sprintf(fout,"%s_%d-%d.x_sp",argv[1],start,end);
		FOUT=fopen(fout,"w");
		i_x = 0;

		while(fscanf(FIN_CH1,"%lf\t%lf\t%lf",&fre[i_x],&xr[1],&xi[1]) != EOF && fscanf(FIN_CH2,"%lf\t%lf\t%lf",&fre[i_x],&xr[2],&xi[2]) != EOF){
			xi[1] = -1.00 * xi[1];
			xrr[i_x] = (xr[1] * xr[2] - xi[1] * xi[2]);
			xii[i_x] = (xr[1] * xi[2] + xi[1] * xr[2]);
			i_x++;
		}
		i_x_max = i_x;

		for(i_x=0;i_x<i_x_max;i_x++){
			fprintf(FOUT,"%lf\t%lf\t%lf\n", fre[i_x],xrr[i_x],xii[i_x]);
		}

		fclose(FIN_CH1);
		fclose(FIN_CH2);
		fclose(FOUT);
    }


    /*################Sum all the data (power value) for each frequency################*/

    /* EMG_CH */
    for(i_p=0;i_p<(INTERVAL/2);i_p++){
		yy[i_p] = 0;
		xx[i_p] = 0;
	}
	i_f = 0;

    for(i=0;i<(total_data /INTERVAL);i++){

		start = i * INTERVAL;
		end = (i+1) * INTERVAL;

		sprintf(fin,"%s_ch%d_%d-%d.fftp",argv[1],EMG_CH,start,end);

		if((FIN=fopen(fin,"r"))==NULL){
			fprintf(stderr, "I can't find %s. I'm sorry !! \n",fin);
			exit(1);
		}

		i_p = 0;
		while(fscanf(FIN,"%lf\t%lf", &fre[i_p], &y[i_f][i_p]) != EOF){
			yy[i_p] += y[i_f][i_p];
			xx[i_p] += sqrt(y[i_f][i_p]);
			i_p++;
		}
		i_f++;
		fclose(FIN);
    }

    i_p_max = i_p;
    i_f_max = i_f;

    sprintf(fout,"%s-%d_ch%d.linuxcsd.px",argv[1],time,EMG_CH);
    sprintf(fout_power,"%s-%d_ch%d.linuxcsd.px-power",argv[1],time,EMG_CH);

    FOUT = fopen(fout,"w");
    FOUT_POWER = fopen(fout_power,"w");

    for(i_p = 0; i_p < i_p_max; i_p++){
		fprintf(FOUT,"%lf\t%lf\n", fre[i_p], (yy[i_p]/(i_f_max * INTERVAL * 0.001)));
		fprintf(FOUT_POWER,"%lf\t%lf\n", fre[i_p], (xx[i_p]/i_f_max));
    }
    fclose(FOUT);
    fclose(FOUT_POWER);

    /* EEG_CH */

    for(i_p=0;i_p<(INTERVAL/2);i_p++){
		yy[i_p] = 0;
		xx[i_p] = 0;
    }
    i_f = 0;

    for(i=0;i<(total_data / INTERVAL);i++){

		start = i * INTERVAL;
		end = (i+1) * INTERVAL;

		sprintf(fin,"%s_ch%d_%d-%d.fftp",argv[1],EEG_CH,start,end);

		if((FIN=fopen(fin,"r"))==NULL){
			fprintf(stderr, "I can't find %s. I'm sorry !! \n",fin);
			exit(1);
		}

		i_p = 0;
		while(fscanf(FIN,"%lf\t%lf", &fre[i_p], &y[i_f][i_p]) != EOF){
			yy[i_p] += y[i_f][i_p];
			xx[i_p] += sqrt(y[i_f][i_p]);
			i_p++;
		}
		i_f++;
		fclose(FIN);
	}

    i_p_max = i_p;
    i_f_max = i_f;

    sprintf(fout,"%s-%d_ch%d.linuxcsd.py",argv[1],time,EEG_CH);
    sprintf(fout_power,"%s-%d_ch%d.linuxcsd.py-power",argv[1],time,EEG_CH);

    FOUT = fopen(fout,"w");
    FOUT_POWER = fopen(fout_power,"w");

    for(i_p = 0; i_p < i_p_max; i_p++){
		fprintf(FOUT,"%lf\t%lf\n", fre[i_p], (yy[i_p]/(i_f_max * INTERVAL * 0.001)));
		fprintf(FOUT_POWER,"%lf\t%lf\n", fre[i_p], (xx[i_p]/i_f_max));
    }
    fclose(FOUT);
    fclose(FOUT_POWER);


    /* X_SP */

    for(i_p=0;i_p<1000;i_p++){
		yy[i_p] = 0;
		xx[i_p] = 0;
    }
    i_f = 0;

    for(i=0;i<(total_data /INTERVAL);i++){

		start = i * INTERVAL;
		end = (i+1) * INTERVAL;

		sprintf(fin,"%s_%d-%d.x_sp",argv[1],start,end);

		if((FIN=fopen(fin,"r"))==NULL){
			fprintf(stderr, "I can't find %s. I'm sorry !! \n",fin);
			exit(1);
		}

		i_p = 0;

		while(fscanf(FIN,"%lf\t%lf\t%lf", &fre[i_p], &xrr[i_p], &xii[i_p]) != EOF){
			xx[i_p] += xrr[i_p];
			yy[i_p] += xii[i_p];
			i_p++;
		}

		i_f++;
		fclose(FIN);
    }

    i_p_max = i_p;
    i_f_max = i_f;

    sprintf(fout,"%s-%d_ch%dch%d.linuxcsd.xsp",argv[1],time,EMG_CH,EEG_CH);

    FOUT = fopen(fout,"w");

    for(i_p = 0; i_p < i_p_max; i_p++){
		fprintf(FOUT,"%lf\t%lf\t%lf\n", fre[i_p], (xx[i_p]/(i_f_max * INTERVAL * 0.001)), (yy[i_p]/(i_f_max * INTERVAL * 0.001)));
    }
    fclose(FOUT);


    /*################Coherence & Phase################*/

	sprintf(fin_px,"%s-%d_ch%d.linuxcsd.px",argv[1],time,EMG_CH);
	sprintf(fin_py,"%s-%d_ch%d.linuxcsd.py",argv[1],time,EEG_CH);
	sprintf(fin_xsp,"%s-%d_ch%dch%d.linuxcsd.xsp",argv[1],time,EMG_CH,EEG_CH);

	if((FIN_PX=fopen(fin_px,"r"))==NULL || (FIN_PY=fopen(fin_py,"r"))==NULL || (FIN_XSP=fopen(fin_xsp,"r"))==NULL){
		fprintf(stderr, "I can't find %s , %s, and %s. I'm sorry !! \n",fin_px,fin_py,fin_xsp);
		exit(1);
	}

	sprintf(fout,"%s-%d_ch%dch%d.linuxcsd",argv[1],time,EMG_CH,EEG_CH);
	FOUT = fopen(fout,"w");

	while(fscanf(FIN_XSP,"%lf\t%lf\t%lf", &fre[i], &cross_r, &cross_i) != EOF && fscanf(FIN_PX,"%lf\t%lf", &fre[i], &px) != EOF && fscanf(FIN_PY,"%lf\t%lf", &fre[i], &py) != EOF){
		cohi = (cross_r * cross_r + cross_i * cross_i) / (px * py);
		fprintf(FOUT, "%lf\t%lf\n",fre[i],cohi);
	}
	fclose(FIN_PX);
	fclose(FIN_PY);
	fclose(FIN_XSP);
	fclose(FOUT);

	system("rm *.fftri");
	system("rm *.fftp");
	system("rm *.x_sp");
}

void Manual(int argc){
    if(argc != 4){
        printf("To run programme type  \n");
        printf("                        ./FFT_COH2 <filename(-#.linuxcsd)> <EEG_CH> <time>\n\n");
        exit(1);
    }
}