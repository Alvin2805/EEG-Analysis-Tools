clear
clc
close all

%load filter.mat

for trial=1:5

file=fopen(['mutou2014317-' num2str(trial) '.eeg_emg'],'r+');
[data,count]=fread(file,'uint16');
fclose(file);
n_chan=32;
start=3;
epoch=58;
intervaldata=1024;
dataraw=data(start:end);
n=length(dataraw);
interval=60000;
step=1;
raw=zeros(n_chan,interval);

SamplingRate=1000;
Ts=1/SamplingRate;

beginint=1;
endint=interval;
while step <=n_chan
    t=dataraw(beginint:endint)';
    raw(step,:)=t;
    beginint=endint+1;
    endint=beginint+interval-1;
    step=step+1;
end

time=0:Ts:interval*Ts;
time=time(2:end);

for i=1:n_chan
    if i==20
        raw(i,:)=(raw(i,:)-32768)*2500*1/32768;
    else
        raw(i,:)=(raw(i,:)-32768)*500*1/32768;
    end
end

%eeg_chan=filter(Hd,raw(10,:));
%emg_chan=filter(emg,raw(20,:));

%delay_eeg=mean(grpdelay(Hd));
%delay_emg=mean(grpdelay(emg));
%fix_eeg=eeg_chan;
%fix_emg=emg_chan;
%fix_eeg(1:delay_eeg)=[];
%fix_emg(1:delay_emg)=[];
fix_eeg=raw(10,1:epoch*intervaldata);
fix_emg=raw(20,1:epoch*intervaldata);
nfft=2048;
N=0;
Fs=1000;
[cohr,f]=mscohere(fix_eeg,fix_emg,1024,0,nfft,1000);

ccc(:,trial)=cohr;

end


indf=find(f>15 & f<=50);
uni=mean(ccc,2);
plot(f,uni);
xlim([0,50]);
fff=max(uni(indf));
ggg=find(uni==fff);
title(['F=' num2str(f(ggg)) ', Cohr=' num2str(fff)])