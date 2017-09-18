clear;

d1=input('Please type hi_keiken or keiken:','s');
d2=input('Please type subject:','s');
d3=input('Please type the date:','s');


coh_all=zeros(1,19);

%Calcrate peak frequency of C3
filename=sprintf('hakei/%s/%s%s/%s%s-1_ch19ch6.linuxcsd',d1,d2,d3,d2,d3);
%If you want to change the channel,changing ch6 to your choice. 
data=load(filename);

frequency=data(13:31,1);
coherence=data(13:31,2);

peak_frequency=find(coherence==max(coherence))+12;


%Calcrate each coherence
for ch=1:19
filename=sprintf('hakei/%s/%s%s/%s%s-1_ch19ch%d.linuxcsd',d1,d2,d3,d2,d3,ch-1);
ch_data=load(filename);

coh_all(1,ch)=ch_data(peak_frequency,2);
    
end

savefile=sprintf('%s%s',d2,d3);
draw_topoC( coh_all/coh_all(9)*100, savefile, 0,100 );
%If you want to change the channel,changing 7 to your choice. 
%seikikawosite 100%hyouji

%draw_topoC( coh_all, savefile, 0,coherence(max) );
%ataiwo hyoujisurubaai


