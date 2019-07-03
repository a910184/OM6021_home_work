function [  ] = REMO_close( Port )

fclose(Port)
delete(Port)

end
