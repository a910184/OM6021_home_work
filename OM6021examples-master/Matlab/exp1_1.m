%%
% @file exp1_1.m
% @author LiYu
% @date 2017.11.26
% @brief Example of getting array data by using HMI function.
%
% Can run with "exp1_1.c".

%%
% 清除之前變數
clc; clear;

% 開啟串列埠
port_num = 8;
port = REMO_open(port_num);

% 取得字串
s1 = fscanf(port,'%s');
s2 = fscanf(port,'%s');

% 發送字串
fprintf(port, 'Hello');
res = fscanf(port,'%s');
% ASA_M128 會把 scanf 接收到的字串重新發送一遍，故此會接收到 'Hello'

% 接收矩陣資料
type = 8; bytes = 4*5; % 5 個 f32
f32_data  = REMO_get(port, type, bytes);
type = 5; bytes = 2*5; % 5 個 ui8
ui16_data = REMO_get(port, type, bytes);
type = 2; bytes = 4*8; % 8 個 i32
i32_data  = REMO_get(port, type, bytes);

% 取得字串
s3 = fscanf(port,'%s');

% 修改資料後，送回M128
for i = 1:5
    f32_data(i) = f32_data(i) + 1;
end
type = 8; bytes = 4*5; % 5 個 f32
REMO_put(port, type, bytes, f32_data);

% 接收矩陣資料
f32_data_re = REMO_get(port, type, bytes);

% 關閉串列埠
REMO_close(port);
