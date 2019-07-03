clear all;clc;
fip = fopen('E:\ASA_DATA\ADC.BIN','rb');
L=1000;Fs=8000;
HZ = Fs*(1:L)/L;
[a,num]=fread(fip,[1,1000],'int8');
fclose(fip);
figure(1) 
plot(a')
f=fft(a');
figure(2)
plot(HZ,abs(f))