%%
% @file exp1_2.m
% @author LiYu
% @date 2017.11.26
% @brief Example of getting array data by using HMI function.
%
% Can run with "exp1_2.c".

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

% 接收結構資料
Bytes = 1*10 + 4*10 + 4*5;
[data,FormatString,error] = REMO_FORM_get(port,Bytes);
data

% 改變資料
data.type1(1) = data.type1(1) + 1;

% 發送結構資料
REMO_FORM_put(port,FormatString,Bytes,data);

% 接收結構資料
[data_re,FormatString,error] = REMO_FORM_get(port,Bytes);
data_re

% 關閉串列埠
REMO_close(port);
