deviceReader = audioDeviceReader;
y = [];
Fs = 48000;
while 1

    while length(y) < 48000
        mySignal = deviceReader();
        %myProcessedSignal = process(mySignal);
        y = cat(1,y,mySignal);
    end
    y = single(y(1:48000));
    myStr = matClassModel(y);
    disp(string(myStr))
    y = [];
end
disp("End Signal Input")

release(deviceReader)