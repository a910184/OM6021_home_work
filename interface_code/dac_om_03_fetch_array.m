clear;clc;
delete(instrfindall);
COM = 'COM8';
% set commuication
Port = serial(COM);
set(Port,'BaudRate',38400);
set(Port,'InputBufferSize',800000);
set(Port, 'FlowControl', 'none');
set(Port, 'Parity', 'none');
set(Port, 'DataBits', 8);
set(Port, 'StopBit', 1);
set(Port, 'Timeout',1000);
fopen(Port);
%fileID = fopen('ADCDATA.bin');
rec_sec = 1;

rec1 = fscanf(Port, '%s');
fprintf(Port, '%d\n' , rec_sec);
rec2 = fscanf(Port, '%s')
count = fscanf(Port, 'Sample %ld sample  \n');
count = double(count) * 2;
A = fread(Port, count,'uint16')
A1 = A(1:2:count)
A2 = A(2:2:count)

%A = fread(fileID, 'int8');
figure(1);
subplot(2,1,1);
plot(A1);
title('Channel 1');
hold on;
subplot(2,1,2);
plot(A2);
title('Channel 2');
hold on;

%fclose(fileID)
fclose(Port);
