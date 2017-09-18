clear;

N      = 1024;       % Data point for FFT
Fs     = 1000;       % Sampling Freq.
df     = Fs/N;       % bunkainou
num_tmp = int8((50/df)+1);   %50Hz made

directory	=	'/lab/brain/ushijima/coherence';
file__		=	input('Please type file :','s');
file_		=	input('Please type file day:','s');
%savefile	=	input('Please type savefile:','s');

%file__		=	'fujiyama';
%file_		=	'fujiyama2014513';
huka		=	['10';'15';'30';];
for tmp=1:3
	file		=	strcat(file__,'/',file_,huka(tmp,:),'/',file_);
	filename	=	strcat(directory,'/',file,'-1_ch19ch9.linuxcsd');

	coh_all		=	zeros(1,19);
	data		=	load(filename);
	tmp2=0;
	for 	i=12:53
		if (data(i-1,2) < data(i,2)) && (data(i+1,2) < data(i,2) && data(i,2)>0.02)
			tmp2=tmp2+1;
			if tmp2==1
				peak_frequency = i;
			else
				peak_frequency = [peak_frequency;i;];
			end
		end
	end
	temp =size(peak_frequency);
	for  i=1:temp(1)
		if strcmp(  input( strcat('Select freq (y or n):huka',huka(tmp,:),',',num2str(data(peak_frequency(i),1)),'Hz :') ,'s'),'y')
			low_freq  = input(strcat('Low_frequency  :',num2str(peak_frequency(i)),' :' ));
			high_freq = input(strcat('High_frequency :',num2str(peak_frequency(i)),' :' ));
			savefile	=	strcat('Topography/',file_,'_',huka(tmp,:),'_',num2str(data(peak_frequency(i),1)),'Hz_',num2str(low_freq*df),'-',num2str(high_freq*df),'Hz');
			%Calcrate each coherence
			for ch=1:19
				filename		=	strcat(directory,'/',file,'-1_ch19ch',num2str(ch-1),'.linuxcsd');

				ch_data			=	load(filename);
				peak_frequency_2	=	find(ch_data(low_freq:high_freq,2)==max(ch_data(low_freq:high_freq,2)))+low_freq-1;
				coh_all(1,ch)		=	ch_data(peak_frequency_2,2);
			        
			end


			draw_topoC2( coh_all/coh_all(10)*100, savefile, 0,100 );
		end
	end
end