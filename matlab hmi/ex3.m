clc;
clear;
port_num = 10;
port = REMO_open(port_num);


s1 = fscanf(port,'%s');
fprintf(port,'1' );
s1 = fscanf(port,'%s');
result0v=fscanf(port,'%d');
result1v=fscanf(port,'%d');
delta = (result1v-result0v)
gain = 1.23/delta
s1 = fscanf(port,'%s');
fprintf(port,'0' );
s1 = fscanf(port,'%s');
REMO_close(port);