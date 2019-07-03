function [error] = REMO_put(Port,Type,Bytes,data)

error = 0;
if (Bytes > 32)
    error = 1;
    warning('資料過多bytes>32');
    return;
end

persistent BatchesBytes ;
persistent ResumingMode ;

if (isempty(ResumingMode)) %It does not already exist
    ResumingMode = 0;
end

char PackBytes;
if (Bytes <= 32 && ResumingMode == 0)
    PackBytes = Bytes;
elseif( Bytes > 32 && ResumingMode == 0)
    BatchesBytes = Bytes;
    PackBytes = 32;
    ResumingMode = 1;
    BatchesBytes = BatchesBytes -PackBytes;
elseif( Bytes == 0 && ResumingMode == 1 )
    PackBytes = 32;
    ResumingMode = 1;
    BatchesBytes = BatchesBytes -PackBytes;
    if (BatchesBytes > 32)
        ResumingMode = 1;
    else
        ResumingMode = 2;
    end
elseif ( Bytes > 0 && ResumingMode == 2)
    PackBytes = Bytes;
    ResumingMode = 0;
else
    warning('error type');
    return;
end

error = 0;

switch Type
    case 0  % int8
        type = 'int8';
        TypeSize = 1;
        Cdata = typecast(int8(data),'uint8');
    case 1  % int16
        type = 'int16';
        TypeSize = 2;
        Cdata = typecast(int16(data),'uint8');
    case 2  % int32
        type = 'int32';
        TypeSize = 4;
        Cdata = typecast(int32(data),'uint8');
    case 3  % int64
        type = 'int64';
        TypeSize = 4;
        Cdata = typecast(int64(data),'uint8');
    case 4  % uint8
        type = 'uint8';
        TypeSize = 1;
        Cdata = typecast(uint8(data),'uint8');
    case 5  % uint16
        type = 'uint16';
        TypeSize = 2;
        Cdata = typecast(uint16(data),'uint8');
    case 6  % uint32
        type = 'uint32';
        TypeSize = 4;
        Cdata = typecast(uint32(data),'uint8');
    case 7  % uint64
        type = 'uint64';
        TypeSize = 4;
        Cdata = typecast(uint64(data),'uint8');
    case 8  % float32
        type = 'single';
        TypeSize = 4;
        Cdata = typecast(single(data),'uint8');
    case 9  % float64
        type = 'double';
        TypeSize = 8;
        Cdata = typecast(double(data),'uint8');
    otherwise
        warning('error type');
        return
end

fwrite(Port,hex2dec('AB'),'uint8');
fwrite(Port,hex2dec('AB'),'uint8');
fwrite(Port,hex2dec('AB'),'uint8');
fwrite(Port,Type,'uint8');
fwrite(Port,PackBytes,'uint8');

% get binary data in uint8 form ASA_PC
CheckSum = Bytes;
for i = 1:Bytes
    fwrite(Port,Cdata(i),'uint8');
    CheckSum = CheckSum + Cdata(i);
end

% get check value form ASA_PC
CheckSum = rem(CheckSum,256);
fwrite(Port,CheckSum,'uint8');


end
