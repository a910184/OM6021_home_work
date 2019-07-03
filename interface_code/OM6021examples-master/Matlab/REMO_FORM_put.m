function [error] = REMO_FORM_put(Port,FormatString,Bytes,data)

error = 0;
CheckSum = 0;

fwrite(Port,hex2dec('AB'),'uint8');
fwrite(Port,hex2dec('AB'),'uint8');
fwrite(Port,hex2dec('AB'),'uint8');

FormatBytes = length(FormatString);
TotalBytes = 1+FormatBytes+Bytes;
if ( TotalBytes >255 )
    warning('資料結構字串長度過長');
    return;
end
fwrite(Port,TotalBytes,'uint8');
fwrite(Port,FormatBytes,'uint8');

%  FormatString
[Format,TypeNum,StructSize,error] = FormatGet(FormatString);
CheckSum = CheckSum + TotalBytes;
CheckSum = FormatBytes;

% put FormatString
for i = 1:FormatBytes
    fwrite(Port,uint8(FormatString(i)),'uint8');
    CheckSum = CheckSum + FormatString(i);
end

% put data
names = fieldnames(data);

for n = 1:Bytes/StructSize
    for i = 1:TypeNum
        putdata = cast(getfield(data(n),names{i}) , Format(i).Type);
        Cdata = typecast(putdata,'uint8');
        fwrite(Port,Cdata,'uint8');
        CheckSum = CheckSum +sum(Cdata);
    end
end

% put CheckSum
CheckSum = rem(CheckSum,256);
fwrite(Port,CheckSum,'uint8');


end
