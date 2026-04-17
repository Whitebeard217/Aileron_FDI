S = [0 0 0 1]';
i = 1;
while(1)
    Sensor_data = sim("sm_aileron_actuator",0.01)
    [E,~,~]=preprationtext_M(inp,1);
   % E = cell2mat(E);
    Y(:,i) = net([E;S]);
    
     if ~isempty(find(round(Y(:,i))|[0 0 0 0]>0))
        S =round(Y(:,i));
        clc
        [~,~,State]=preprationtext_M(S,3)
   % binaryVectorToDecimal(S')
        i = i+1;
     end
end