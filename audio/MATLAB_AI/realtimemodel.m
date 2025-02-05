load("binDroneModel.mat");
classes = categorical({'Drone'; 'NoDrone'});

deviceReader = audioDeviceReader;
y = [];
Fs = 44100;

%process = @(x) x.*5; %whatever pre processing we want goes here...

disp("Begin Signal Input...")

while 1
    tic
    while toc<1.1
        mySignal = deviceReader();
        %myProcessedSignal = process(mySignal);
        y = cat(1,y,mySignal);
    end
    Y = repmat(y(:), 1, 2);
    X = HelperMelSpectrograms(Y,Fs);
    scores = predict(netTrained,X);
    [label,score] = scores2label(scores,classes);
    myStr = string(label) + " (Score: " + score + ")";
    disp(myStr)
    y = [];
end
disp("End Signal Input")

release(deviceReader)