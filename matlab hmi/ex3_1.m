clc;
clear;
port_num = 10;
type = 8; bytes = 4*4;
port = REMO_open(port_num);
s1 = fscanf(port,'%s');
fprintf(port,'4000' );
s1 = fscanf(port,'%s');
s1 = fscanf(port,'%s');
for i=1:4000
x1(i) = REMO_get(port, 4, 1);
end
L=4000;Fs=8000;
HZ = Fs*(1:L)/L;
figure(1) 
plot(x1)
f=fft(x1);
figure(2)
plot(HZ,abs(f))
REMO_close(port);