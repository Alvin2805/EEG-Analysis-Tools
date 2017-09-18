function draw_topoC( eeg_data, file_name, v_min, v_max)
%
%% Draw topograph ( contour version )
%
%  draw_topo_c( eeg_data, filename, v_min, v_max)
%
%  file_name -> file name (BMP file)
%  eeg_data  -> [ ch0 ch1 ch2 ch3 ch4 ... ch18]
%             [ Fp1 F7 T3 T5 O1 F3 C3 P3 Fz Cz Pz F4 C4 P4 Fp2 F8 T4 T6 O2] 
%
%  Colorbar range 
%            -> v_min ~ v_max   



% --- Save Path ---
%PATH = '~/topo_contour/topoC_';
PATH = '';

%!rm -rf ~/topo_contour
!mkdir ~/topo_contour

file_name = strcat(PATH, file_name);
file_name = strcat(file_name, '.bmp');


topo = calc_topo(eeg_data);

h = figure('visible', 'off');
colormap(h,'jet')
contourf(topo);
hold on;
[x, y]=meshgrid(6:10:46, 16:10:36);
X = reshape(x, 1,numel(x));
Y = reshape(y, 1,numel(y));
[x, y]=meshgrid(21:10:31, 6:40:46);
x = reshape(x, 1,numel(x));
y = reshape(y, 1,numel(y));
X = [X x];
Y = [Y y];
plot(X,Y, '+k', 'MarkerSize', 10);  % Draw electrodes
rectangle('position',[1 1 50 50], 'curvature', [1,1]);  % Draw outline
axis square;
axis off;
caxis([v_min v_max]);  % color axis -> [min max]
colorbar;
hold off;
print(h, '-dbmp16m', file_name);
close(h);

end
