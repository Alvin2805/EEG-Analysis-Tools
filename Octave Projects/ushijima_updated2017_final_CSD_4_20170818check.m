clear
clc
close all

load pml_data_1;
g=data_pml_1;

filename='mutou2014317';
%desired_time=[0;12000;24000;36000;48000;60000];
desired_time=[0;60000];

start=0;
finish=0;
ntrial=5;
max_volt=100;

nn_data=desired_time(2)-desired_time(1); 
tot_epo=floor(nn_data/1024);

temp_data=zeros(ntrial*tot_epo,1);

n_observe=length(desired_time);
i_observe=0;

%channel structures#
while i_observe < n_observe-1
    i_observe=i_observe+1;
    clear copyeeg copyemg reject_data eeg emg;
    eeg=zeros(20,(nn_data)*ntrial);
    for trial=1:ntrial
        file=fopen([filename '-' num2str(trial) '.eeg_emg'],'r+');
        [data,count]=fread(file,'uint16');
        Fs=1000;
        interval=1024;
        ch=32;
        exp_time=60000;
        ndata=length(data);

        %change time span
        startimed=desired_time(i_observe)+1;
        endtimed=desired_time(i_observe+1);
        n_data_total=(endtimed-startimed)+1;

        data_fix=data(3:ndata);
        step=1;
        i=1;
        raw_data=zeros(32,exp_time);

        while step <= ndata-exp_time
            raw_data(i,:)=data_fix(step:step+exp_time-1);
            step=step+exp_time;
            i=i+1;
        end
        
        for i=1:32
            if i==20
                calc=(raw_data(i,1:exp_time) - 32768) * 2500 * (1.0/32768);
                raw_data(i,:)=calc;
            else
                calc=(raw_data(i,1:exp_time) - 32768) * 500 * (1.0/32768);
                raw_data(i,:)=calc;
            end
        end
      
        data=raw_data(:,startimed:endtimed);  
       
        ep=0;
        idx_reject=0;
        while (ep<floor(n_data_total/interval))
            ep=ep+1;
            startep=(ep-1)*interval+1;
            finishep=(ep)*interval;

            for chh=1:19
                for intt=startep:finishep
                    n_max=abs(data(chh,intt));
                    if (temp_data(ep+((trial-1)*tot_epo)) < n_max)
                        temp_data(ep+((trial-1)*tot_epo)) = n_max;
                    end
                end
            end
        end
        
        fix_eeg=data(1:20,:); 
        
        nn_data_s=((trial-1)*(nn_data))+1;
        nn_data_f=trial*(nn_data);
        plot_eeg(:,nn_data_s:nn_data_f)=fix_eeg;
    end 
    
    eeg=plot_eeg;
    %remove bad epochs
    reject_data=find(temp_data>max_volt);

    for iii=1:length(reject_data)
        n_epoch=reject_data(iii);
        start_epoch_reject=((n_epoch-1)*interval)+1;
        end_epoch_reject=n_epoch*i#000000#000000#000000#000000nterval;
        eeg(:,start_epoch_reject:end_epoch_reject)=100000;
    end
    
    erase_eeg_epoch=find(eeg(1,:)==100000);
    eeg(:,erase_eeg_epoch)=[];
    accept_epoch=temp_data;
    accept_epoch(reject_data)=[];
    tot_epoch=length(accept_epoch);
    
    eeg=eeg';
    emg=eeg(:,20);
    eeg=eeg(:,1:19);
    
    M=[1.000000 ,0.015261 ,0.012221 ,0.004491 ,-0.004710 ,-0.011927 ,0.008521 ,0.003122 ,-0.003383 ,0.010080 ,-0.000152 ,-0.009974 ,0.003122 ,-0.003383 ,-0.008530 ,0.012221 ,0.004491 ,-0.004710 ,-0.011927 ,-0.014637 ;1.000000 ,0.012221 ,0.015261 ,0.012221 ,0.004491 ,-0.004710 ,0.010620 ,0.008521 ,0.003122 ,0.006115 ,-0.000152 ,-0.006264 ,-0.003383 ,-0.008530 ,-0.010471 ,0.004491 ,-0.004710 ,-0.011927 ,-0.014637 ,-0.011927 ;1.000000 ,0.004491 ,0.012221 ,0.015261 ,0.012221 ,0.004491 ,0.008521 ,0.010620 ,0.008521 ,-0.000152 ,-0.000152 ,-0.000152 ,-0.008530 ,-0.010471 ,-0.008530 ,-0.004710 ,-0.011927 ,-0.014637 ,-0.011927 ,-0.004710 ;1.000000 ,-0.004710 ,0.004491 ,0.012221 ,0.015261 ,0.012221 ,0.003122 ,0.008521 ,0.010620 ,-0.006264 ,-0.000152 ,0.006115 ,-0.010471 ,-0.008530 ,-0.003383 ,-0.011927 ,-0.014637 ,-0.011927 ,-0.004710 ,0.004491 ;1.000000 ,-0.011927 ,-0.004710 ,0.004491 ,0.012221 ,0.015261 ,-0.003383 ,0.003122 ,0.008521 ,-0.009974 ,-0.000152 ,0.010080 ,-0.008530 ,-0.003383 ,0.003122 ,-0.014637 ,-0.011927 ,-0.004710 ,0.004491 ,0.012221 ;1.000000 ,0.008521 ,0.010620 ,0.008521 ,0.003122 ,-0.003383 ,0.015261 ,0.013733 ,0.009800 ,0.011982 ,0.010620 ,0.002935 ,0.005046 ,0.001273 ,-0.000152 ,0.003122 ,-0.003383 ,-0.008530 ,-0.010471 ,-0.008530 ;1.000000 ,0.003122 ,0.008521 ,0.010620 ,0.008521 ,0.003122 ,0.013733 ,0.015261 ,0.013733 ,0.007409 ,0.010620 ,0.007409 ,0.001273 ,-0.000152 ,0.001273 ,-0.003383 ,-0.008530 ,-0.010471 ,-0.008530 ,-0.003383 ;1.000000 ,-0.003383 ,0.003122 ,0.008521 ,0.010620 ,0.008521 ,0.009800 ,0.013733 ,0.015261 ,0.002935 ,0.010620 ,0.011982 ,-0.000152 ,0.001273 ,0.005046 ,-0.008530 ,-0.010471 ,-0.008530 ,-0.003383 ,0.003122 ;1.000000 ,0.010080 ,0.006115 ,-0.000152 ,-0.006264 ,-0.009974 ,0.011982 ,0.007409 ,0.002935 ,0.015261 ,0.010620 ,-0.000152 ,0.011982 ,0.007409 ,0.002935 ,0.010080 ,0.006115 ,-0.000152 ,-0.006264 ,-0.009974 ;1.000000 ,-0.000152 ,-0.000152 ,-0.000152 ,-0.000152 ,-0.000152 ,0.010620 ,0.010620 ,0.010620 ,0.010620 ,0.015261 ,0.010620 ,0.010620 ,0.010620 ,0.010620 ,-0.000152 ,-0.000152 ,-0.000152 ,-0.000152 ,-0.000152 ;1.000000 ,-0.009974 ,-0.006264 ,-0.000152 ,0.006115 ,0.010080 ,0.002935 ,0.007409 ,0.011982 ,-0.000152 ,0.010620 ,0.015261 ,0.002935 ,0.007409 ,0.011982 ,-0.009974 ,-0.006264 ,-0.000152 ,0.006115 ,0.010080 ;1.000000 ,0.003122 ,-0.003383 ,-0.008530 ,-0.010471 ,-0.008530 ,0.005046 ,0.001273 ,-0.000152 ,0.011982 ,0.010620 ,0.002935 ,0.015261 ,0.013733 ,0.009800 ,0.008521 ,0.010620 ,0.008521 ,0.003122 ,-0.003383 ;1.000000 ,-0.003383 ,-0.008530 ,-0.010471 ,-0.008530 ,-0.003383 ,0.001273 ,-0.000152 ,0.001273 ,0.007409 ,0.010620 ,0.007409 ,0.013733 ,0.015261 ,0.013733 ,0.003122 ,0.008521 ,0.010620 ,0.008521 ,0.003122 ;1.000000 ,-0.008530 ,-0.010471 ,-0.008530 ,-0.003383 ,0.003122 ,-0.000152 ,0.001273 ,0.005046 ,0.002935 ,0.010620 ,0.011982 ,0.009800 ,0.013733 ,0.015261 ,-0.003383 ,0.003122 ,0.008521 ,0.010620 ,0.008521 ;1.000000 ,0.012221 ,0.004491 ,-0.004710 ,-0.011927 ,-0.014637 ,0.003122 ,-0.003383 ,-0.008530 ,0.010080 ,-0.000152 ,-0.009974 ,0.008521 ,0.003122 ,-0.003383 ,0.015261 ,0.012221 ,0.004491 ,-0.004710 ,-0.011927 ;1.000000 ,0.004491 ,-0.004710 ,-0.011927 ,-0.014637 ,-0.011927 ,-0.003383 ,-0.008530 ,-0.010471 ,0.006115 ,-0.000152 ,-0.006264 ,0.010620 ,0.008521 ,0.003122 ,0.012221 ,0.015261 ,0.012221 ,0.004491 ,-0.004710 ;1.000000 ,-0.004710 ,-0.011927 ,-0.014637 ,-0.011927 ,-0.004710 ,-0.008530 ,-0.010471 ,-0.008530 ,-0.000152 ,-0.000152 ,-0.000152 ,0.008521 ,0.010620 ,0.008521 ,0.004491 ,0.012221 ,0.015261 ,0.012221 ,0.004491 ;1.000000 ,-0.011927 ,-0.014637 ,-0.011927 ,-0.004710 ,0.004491 ,-0.010471 ,-0.008530 ,-0.003383 ,-0.006264 ,-0.000152 ,0.006115 ,0.003122 ,0.008521 ,0.010620 ,-0.004710 ,0.004491 ,0.012221 ,0.015261 ,0.012221 ;1.000000 ,-0.014637 ,-0.011927 ,-0.004710 ,0.004491 ,0.012221 ,-0.008530 ,-0.003383 ,0.003122 ,-0.009974 ,-0.000152 ,0.010080 ,-0.003383 ,0.003122 ,0.008521 ,-0.011927 ,-0.004710 ,0.004491 ,0.012221 ,0.015261 ;0.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ,1.000000 ];
    a=inv(M);
    a=a';
    
    C=zeros(length(eeg(:,1)),20);
    
    for count=1:length(eeg(:,1))
        z_matrix=zeros(1,20);
        z_matrix(1:19)=eeg(count,:);
        summ=z_matrix;
        
        for mat2=1:20
            b=0;
            for mat=1:20
                b=b+a(mat,mat2)*summ(mat);
            end
            C(count,mat2)=b;
        end
    end
    
    dataC=C(:,2:20); 
    
    for k=1:length(dataC(:,1))
        for i=1:19
            addr(i)=0;
            for j=1:19
                n=g(i,j)*dataC(k,j);
                addr(i)=addr(i)+n;
            end
        end
        for i=1:19
            dataC(k,i)=addr(i);
        end
    end
    
    Cz=dataC;
    
    for chan=1:19
     
        %for EMG renormalization ave
        for i=1:floor(length(emg)/1024)
            emg_start=(i-1)*interval+1;
            emg_finish=i*interval;  

            sum_eeg=0;
            sum_emg=0;
            for j=emg_start:emg_finish
                emg(j)=abs(emg(j));
                sum_emg=sum_emg+emg(j);

                sum_eeg=sum_eeg+Cz(j);
            end
            ave_emg=sum_emg/interval;
            ave_eeg=sum_eeg/interval;
            for j=emg_start:emg_finish
                new_data=emg(j)-ave_emg;
                emg_new(j)=new_data;

                new_data=Cz(j,chan)-ave_eeg;
                eeg_new_temp(j)=new_data;
            end
        end  
        eeg_new(chan,:)=eeg_new_temp;
    end
    
    clear emg eeg;
    emg=emg_new;
    eeg=eeg_new;
    
 
    
    %start for coherence calculation
    
    for chan=1:19
    
        [cohr,f]=mscohere(eeg(chan,:),emg,hanning(interval),0,1024,Fs);
        ind_focus=find(f>0 & f<=50);
  
        focus_cz=cohr(ind_focus);
        focus_f=f(ind_focus);
        maxcohr=max(focus_cz);
        ind_max_cohr=find(focus_cz==maxcohr);
        maxf=focus_f(ind_max_cohr); % peak frequency

       
        if (chan == 10)
            bb_maxf=ind_max_cohr;
        else
            ind_max_cohr=ind_max_cohr;
        end
%         figure(i_observe);
%         plot(f,cohr);
%         xlim([0,50]);
%         %ylim([0,0.05]);
%         title(['F=' num2str(maxf) ',C=' num2str(maxcohr) ',Nepoch=' num2str(floor(tot_epoch))]);
%         xlabel('freq(hz)');
%         ylabel('coherence value');

        cut_data=find(f<=50);

        %csvwrite(['result_data_' num2str(i_observe) '.csv'],[f(cut_data), cohr(cut_data)]);  
        cohrr(chan,:)=cohr(cut_data);
        
    end
end

%drawing brain topography
load chan_ush;
mag=cohrr(:,bb_maxf);
eegplot(mag,chan_ush,[],1,'linear',[]);



% %second _method calculating the RMS and MPF

% t_plot=0:1/1000:(1/1000)*60000;
% t_plot=t_plot(2:end);
% rms_scale=[20,300];
% mpf_scale=[25,37];
% emg=plot_eeg(20,1:60000);
% h1=figure;
% subplot(3,1,1),plot(t_plot,emg);
% title('EMG Data');
% ylim([-1000,1000]);
% idx=1;
% n_shift=12000;
% for i=1:n_shift:length(emg)
%     data_take=emg(i:i+n_shift-1);
%     power_f=fft(data_take)/n_shift;
%     Y=abs(power_f);
%     Y_f=Y(1:n_shift/2+1);
%     f=1000*(0:(n_shift/2))/n_shift;
%     find_f=find(f>0.5 & f<=60);
%     mean_freq=sum(f(find_f).*Y_f(find_f))/sum(Y_f(find_f));
%     Y_power(idx)=mean(mean_freq);
%     res_RMS(idx)=rms(data_take);
%     idx=idx+1;
% end
% 
% subplot(3,1,2),plot([0,12,24,36,48,60],[res_RMS(1),res_RMS]);
% title('RMS');
% ylim([rms_scale(1),rms_scale(2)]);
% subplot(3,1,3),plot([0,12,24,36,48,60],[Y_power(1),Y_power]);
% title('MPF');
% ylim([mpf_scale(1),mpf_scale(2)]);
% csvwrite('add_result_1.csv',[res_RMS',Y_power']);
% saveas(h1,'trial_1.jpg');
% % 
% % 
% % 
% emg=plot_eeg(20,60001:120000);
% h2=figure;
% subplot(3,1,1),plot(t_plot,emg);
% title('EMG Data');
% ylim([-1000,1000]);
% idx=1;
% n_shift=12000;
% for i=1:n_shift:length(emg)
%     data_take=emg(i:i+n_shift-1);
%     power_f=fft(data_take)/n_shift;
%     Y=abs(power_f);
%     Y_f=Y(1:n_shift/2+1);
%     f=1000*(0:(n_shift/2))/n_shift;
%     find_f=find(f>0.5 & f<=60);
%     mean_freq=sum(f(find_f).*Y_f(find_f))/sum(Y_f(find_f));
%     Y_power(idx)=mean(mean_freq);
%     res_RMS(idx)=rms(data_take);
%     idx=idx+1;
% end
% subplot(3,1,2),plot([0,12,24,36,48,60],[res_RMS(1),res_RMS]);
% title('RMS');
% ylim([rms_scale(1),rms_scale(2)]);
% subplot(3,1,3),plot([0,12,24,36,48,60],[Y_power(1),Y_power]);
% title('MPF');
% ylim([mpf_scale(1),mpf_scale(2)]);
% csvwrite('add_result_2.csv',[res_RMS',Y_power']);
% saveas(h2,'trial_2.jpg');
% 
% emg=plot_eeg(20,120001:180000);
% h3=figure;
% subplot(3,1,1),plot(t_plot,emg);
% title('EMG Data');
% ylim([-1000,1000]);
% idx=1;
% n_shift=12000;
% for i=1:n_shift:length(emg)
%     data_take=emg(i:i+n_shift-1);
%     power_f=fft(data_take)/n_shift;
%     Y=abs(power_f);
%     Y_f=Y(1:n_shift/2+1);
%     f=1000*(0:(n_shift/2))/n_shift;
%     find_f=find(f>0.5 & f<=60);
%     mean_freq=sum(f(find_f).*Y_f(find_f))/sum(Y_f(find_f));
%     Y_power(idx)=mean(mean_freq);
%     res_RMS(idx)=rms(data_take);
%     idx=idx+1;
% end
% subplot(3,1,2),plot([0,12,24,36,48,60],[res_RMS(1),res_RMS]);
% title('RMS');
% ylim([rms_scale(1),rms_scale(2)]);
% subplot(3,1,3),plot([0,12,24,36,48,60],[Y_power(1),Y_power]);
% title('MPF');
% ylim([mpf_scale(1),mpf_scale(2)]);
% csvwrite('add_result_3.csv',[res_RMS',Y_power']);
% saveas(h3,'trial_3.jpg');
% 
% emg=plot_eeg(20,180001:240000);
% h4=figure;
% subplot(3,1,1),plot(t_plot,emg);
% title('EMG Data');
% ylim([-1000,1000]);
% idx=1;
% n_shift=12000;
% for i=1:n_shift:length(emg)
%     data_take=emg(i:i+n_shift-1);
%     power_f=fft(data_take)/n_shift;
%     Y=abs(power_f);
%     Y_f=Y(1:n_shift/2+1);
%     f=1000*(0:(n_shift/2))/n_shift;
%     find_f=find(f>0.5 & f<=60);
%     mean_freq=sum(f(find_f).*Y_f(find_f))/sum(Y_f(find_f));
%     Y_power(idx)=mean(mean_freq);
%     res_RMS(idx)=rms(data_take);
%     idx=idx+1;
% end
% subplot(3,1,2),plot([0,12,24,36,48,60],[res_RMS(1),res_RMS]);
% title('RMS');
% ylim([rms_scale(1),rms_scale(2)]);
% subplot(3,1,3),plot([0,12,24,36,48,60],[Y_power(1),Y_power]);
% title('MPF');
% ylim([mpf_scale(1),mpf_scale(2)]);
% csvwrite('add_result_4.csv',[res_RMS',Y_power']);
% saveas(h4,'trial_4.jpg');
% 
% 
% emg=plot_eeg(20,240001:300000);
% h5=figure;
% subplot(3,1,1),plot(t_plot,emg);
% title('EMG Data');
% ylim([-1000,1000]);
% idx=1;
% n_shift=12000;
% for i=1:n_shift:length(emg)
%     data_take=emg(i:i+n_shift-1);
%     power_f=fft(data_take)/n_shift;
%     Y=abs(power_f);
%     Y_f=Y(1:n_shift/2+1);
%     f=1000*(0:(n_shift/2))/n_shift;
%     find_f=find(f>0.5 & f<=60);
%     mean_freq=sum(f(find_f).*Y_f(find_f))/sum(Y_f(find_f));
%     Y_power(idx)=mean(mean_freq);
%     res_RMS(idx)=rms(data_take);
%     idx=idx+1;
%     
% end
% subplot(3,1,2),plot([12,24,36,48,60],[res_RMS(1),res_RMS]);
% title('RMS');
% ylim([rms_scale(1),rms_scale(2)]);
% subplot(3,1,3),plot([12,24,36,48,60],[Y_power(1),Y_power]);
% title('MPF');
% ylim([mpf_scale(1),mpf_scale(2)]);
% csvwrite('add_result_5.csv',[res_RMS',Y_power']);
% saveas(h5,'trial_5.jpg');
