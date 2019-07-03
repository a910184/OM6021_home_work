clc;
clear;
port_num = 10;
port = REMO_open(port_num);
s1 = fscanf(port,'%s');
s2 = fscanf(port,'%s');
fprintf(port, 'Hello');
res = fscanf(port,'%s');
type = 8; bytes = 4*4; % 4 ­Ó f32
temp1  = REMO_get(port, type, bytes);
temp2  = REMO_get(port, type, 8);
temp3  = REMO_get(port, type, 8);
temp4  = REMO_get(port, type, 4);
s3 = fscanf(port,'%s');
a=[0,1,-2,2];
b=[0,1];
c=[-2,4];
d=[1];

REMO_put(port, type, 16, a);
REMO_put(port, type, 8, b);
REMO_put(port, type, 8, c);
REMO_put(port, type, 4, d);
for i=1:100
    u(i) = sin(0.02*pi*i);
    REMO_put(port, type, 4, u(i));
end 

temp1  = REMO_get(port, type, 16);
temp2= REMO_get(port, type, 8);
temp3  = REMO_get(port, type, 8);
temp4  = REMO_get(port, type, 4);
for i=1:100
u(i) = REMO_get(port, type, 4);
end
for i=1:100
x1(i) = REMO_get(port, type, 4);
x2(i) = REMO_get(port, type, 4);
y1(i) = REMO_get(port, type, 4);
end
REMO_close(port);