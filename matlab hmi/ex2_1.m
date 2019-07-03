clc;
clear;
port_num = 10;
type = 8; bytes = 4*4;
port = REMO_open(port_num);
for i=1:100
    u(i) = sin(0.02*pi*i);
    REMO_put(port, type, 4, u(i));
end 
for i=1:100
x1(i) = REMO_get(port, type, 4);
end

REMO_close(port);