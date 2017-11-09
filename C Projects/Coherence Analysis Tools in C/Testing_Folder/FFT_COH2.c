#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "CSD.h"

void Manual(int argc);

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

main(int *argc, char *argv[ ]){

  /*################Memory Allocation and Initialization################*/

    xr = (double *)calloc(INTERVAL,sizeof(double));
    if(xr == NULL){
		puts("xr : Failed allocating the memory");
		exit(1);
	}

    st = (double *)calloc((INTERVAL/4)+1,sizeof(double));
	if(st == NULL){
		puts("st : Failed allocating the memory");
    	exit(1);
    }

    xi = (double *)calloc(INTERVAL,sizeof(double));
    if(xi == NULL){
		puts("xi : Failed allocating the memory");
		exit(1);
    }

    xpower = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xpower == NULL){
		puts("xpower : failed allocating the memory");
		exit(1);
    }

    xrr = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xrr == NULL){
		puts("xrr : Failed allocating the memory");
		exit(1);
    }

    xii = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xii == NULL){
		puts("xii : Failed allocating the memory");
		exit(1);
    }

    fre = (double *)calloc(INTERVAL/2,sizeof(double));
    if(fre == NULL){
		puts("fre : Failed allocating the memory");
		exit(1);
    }

    yy = (double *)calloc(INTERVAL/2,sizeof(double));
    if(yy == NULL){
		puts("yy : Failed allocating the memory");
		exit(1);
    }

    xx = (double *)calloc(INTERVAL/2,sizeof(double));
    if(xx == NULL){
		puts("xx : Failed allocating the memory");
		exit(1);
    }

    y = (double **)calloc(FILESUU*EPOCH,sizeof(double *));
    if(y == NULL){
		puts("y : Failed allocating the memory");
		exit(1);
    }
    for(MAT=0;MAT<(FILESUU*EPOCH);MAT++){
		*(y + MAT) = (double *)calloc(INTERVAL/2,sizeof(double));
		if(*(y + MAT) == NULL){
	    	puts("y : Failed allocating the memory");
	    	exit(1);
		}
    }

    data = (double **)calloc(FILESUU*EPOCH*INTERVAL,sizeof(double *));
    if(data == NULL){
		puts("data : Failed allocating the memory");
		exit(1);
    }
    for(MAT=0;MAT<(FILESUU*EPOCH*INTERVAL);MAT++){
		*(data + MAT) = (double *)calloc(CHANNELSUU+1,sizeof(double));
		if(*(data + MAT) == NULL){
			puts("data : Failed allocating the memory");
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

//  system("rm *.fftri");
//  system("rm *.fftp");
//  system("rm *.x_sp");

}

void Manual(int argc){
    if(argc != 4){
        printf("To run programme type  \n");
        printf("                        ./FFT_COH2 <filename(-#.linuxcsd)> <EEG_CH> <time>\n\n");
        exit(1);
    }
}
