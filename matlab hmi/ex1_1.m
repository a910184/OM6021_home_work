clc;
clear;
port_num = 12;
type = 8; bytes = 4*4; 
port = REMO_open(port_num);
for i=1:100
x1(i) = REMO_get(port, type, 4);
x2(i) = REMO_get(port, type, 4);
y1(i) = REMO_get(port, type, 4);
end
REMO_close(port);