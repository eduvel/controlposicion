
close all
%% Periodo de muestreo
Ts =2;%0.05;
Tau=130;
%Total de muestras
N=2^14-1;%2000;%85000;
%Vector de tiempo
t=(0:Ts:(2*N-1)*Ts);
%Entrada Pseudoaleatoria
Entrada=idinput(2*N,'PRBS',[0 1/Tau],[30 70]);%0.0005],[400 800]);
Entrada=Entrada(4200:2*N-2000);%(5001:85000);
t=t(0001:2*N-6199);%80000);