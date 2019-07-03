function [data,FormatString,error] = REMO_FORM_get(Port,Bytes)

error = 0;
CheckSum = 0;

% get pre-check form ASA_PC
for i = 1:3
    check = fread(Port,1,'uint8');
    if check ~= hex2dec('BB');
        error = 2;
        warning('«Ê¥]ÀY¿ù»~');
        return
    end
end

% get TotalBytes
GetTotalBytes = fread(Port,1,'uint8');
CheckSum = GetTotalBytes;

% get FormatBytes
GetFormatBytes = fread(Port,1,'uint8');
CheckSum = CheckSum + GetFormatBytes;

% get data bytes
GetBytes = GetTotalBytes-GetFormatBytes;

% get FormatBytes
for i = 1:GetFormatBytes
    FormatString(i) = fread(Port,1,'uint8');
    CheckSum = CheckSum + FormatString(i);
end
FormatString = char(FormatString);
[Format,TypeNum,StructSize,error] = FormatGet(FormatString);

% get binary data in uint8 form ASA_PC
for i = 1:Bytes
    Cdata(i) = fread(Port,1,'uint8');
    CheckSum = CheckSum + Cdata(i);
end

% get check value form ASA_PC
CheckSum = rem(CheckSum,256);
GetCheckSum = fread(Port,1,'uint8');

% trans binary data to struct
i = 1;
for n = 1 : Bytes/StructSize
    for j = 1:TypeNum
        l = i + Format(j).TypeSize*Format(j).Num - 1;
        arraydata = typecast( uint8(Cdata( i:l )) , Format(j).Type);
        i = l+1;
        celldata(j,n)= {arraydata};
    end
end
data = cell2struct(celldata,{Format.Name},1);

if GetBytes~=Bytes+1
    error = 3;
    warning('Bytes¿ù»~');
    return;
end

if CheckSum ~= GetCheckSum
    error = 4;
    warning('CheckSum¿ù»~');
    return;
end

end
