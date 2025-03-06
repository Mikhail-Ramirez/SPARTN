deviceReader = audioDeviceReader;
y = [];
Fs = 44100;

%process = @(x) x.*5; %whatever pre processing we want goes here...

disp("Begin Signal Input...")

while 1
    tic
    while toc<1.2
        mySignal = deviceReader();
        %myProcessedSignal = process(mySignal);
        y = cat(1,y,mySignal);
    end
    y = single(y(1:44100));
    myStr = matClassModel(y);
    disp(string(myStr))
    y = [];
end
disp("End Signal Input")

release(deviceReader)