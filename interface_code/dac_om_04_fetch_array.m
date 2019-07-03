clc;
clear;
com_handle = REMO_open(8)

send_samples = 10000;

t = linspace(0,20*pi, send_samples);

output_1 = floor(2047*sin(t)+2047);
output_2 = floor(2047*cos(t)+2047);

rec1 = fscanf(com_handle, '%s');
fprintf(com_handle, '%d\n' , send_samples);
rec2 = fscanf(com_handle, '%s')
count = fscanf(com_handle, 'play %d samples \n');

[mat_data str_data] = REMO_FORM_get(com_handle,4);

for n=1:send_samples
    mat_data.type1(1) = output_1(n);
    mat_data.type1(2) = output_2(n);
    REMO_FORM_put(com_handle,str_data,4,mat_data);
end

fclose(com_handle)
