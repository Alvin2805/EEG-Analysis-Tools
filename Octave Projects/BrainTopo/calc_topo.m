function [ topo ] = calc_topo( eeg_data )
%
%% Convert eeg-data into topograph-data
%
%  [topo] = draw_topo( eeg_data, normal, v_max, v_min)
%
%   normal   -> not 0: normalization    0: max, min voltage
%   eeg_data -> [ ch0 ch1 ch2 ch3 ch4 ... ch18]
%             [ Fp1 F7 T3 T5 O1 F3 C3 P3 Fz Cz Pz F4 C4 P4 Fp2 F8 T4 T6 O2] 



% -- EEG data set ( channel sort ) --
ChData  = {'Fp1', 'F7', 'T3', 'T5', 'O1', 'F3', 'C3', 'P3', 'Fz', 'Cz', 'Pz', 'F4', 'C4', 'P4', 'Fp2', 'F8', 'T4', 'T6', 'O2'};
ChOrder = {'F7', 'T3', 'T5', 'F3', 'C3', 'P3', 'Fz', 'Cz', 'Pz', 'F4', 'C4', 'P4', 'F8', 'T4', 'T6', 'Fp1', 'O1', 'Fp2', 'O2'};
ChNo = zeros(length(ChData));
for i = 1:length(ChNo)
    ChNo(:,i) = strcmpi(ChData, ChOrder(i));
end
ChNo = (ChNo*diag(1:length(ChOrder)))';
ChNo = ChNo(ChNo~=0);
EEG(ChNo) = eeg_data;


% --- interpolation ---
EEG(20:23) = (eeg_data([1 5 15 19])*2+eeg_data([2 4 16 18])*3)/5;

[X, Y] = meshgrid(2:2:10, 8:-2:4);
X = reshape(X, 1,numel(X));
Y = reshape(Y, 1,numel(Y));
[x, y]=meshgrid(5:2:7, 10:-8:1);
x = reshape(x, 1,numel(x));
y = reshape(y, 1,numel(y));
X = [X x];
Y = [Y y];
[x, y]=meshgrid(2:8:10, 10:-8:2);
x = reshape(x, 1,numel(x));
y = reshape(y, 1,numel(y));
X = [X x];
Y = [Y y];

EEG = [EEG, EEG(X==2)/2];
X = [X, ones(1,5)*1];
Y = [Y, Y(X==2)];
EEG = [EEG, EEG(Y==2)/2];
X = [X, X(Y==2)];
Y = [Y, ones(1,5)*1];
EEG = [EEG, EEG(X==10)/2];
X = [X, ones(1,6)*11];
Y = [Y, Y(X==10)];
EEG = [EEG, EEG(Y==10)/2];
X = [X, X(Y==10)];
Y = [Y, ones(1,6)*11];

[XI, YI] = meshgrid(1:0.2:11);
topo = griddata(X, Y, EEG, XI, YI);

end

