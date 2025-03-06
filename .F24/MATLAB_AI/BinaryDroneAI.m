% Create audioDatastore that points to training data
dataset = fullfile("AudioDataset");
ads = audioDatastore(dataset,IncludeSubfolders=true,LabelSource="foldernames");

% Split ads into training dataset and validation dataset
[adsTrain, adsVal] = splitEachLabel(ads,0.8,"randomized");

%call read to get data from set
[data, adsInfo] = read(adsTrain);
data = data./max(data,[],"all");

% https://www.mathworks.com/help/audio/ug/acoustic-scene-recognition-using-late-fusion.html

% "Use melSpectrogram to transform the data into a compact frequency-domain
% representation. Define parameters for the mel spectrogram as suggested by
% [3]."
fs = adsInfo.SampleRate;
windowLength = 2048;
samplesPerHop = 1024;
samplesOverlap = windowLength - samplesPerHop;
fftLength = 2*windowLength;
numBands = 128;

% To apply the feature extraction steps to all files in the datastores,
% create transform datastores and specify the HelperSegmentedMelSpectrograms
% function as the transform
tdsTrain = transform(adsTrain,@(x){HelperMelSpectrograms(x,fs, ...
    WindowLength=windowLength, ...
    HopLength=samplesPerHop, ...
    NumBands=numBands, ...
    FFTLength=fftLength)});

outputLocation = fullfile("specDataset");
writeall(tdsTrain,outputLocation,WriteFcn=@myCustomWriter,UseParallel=true);

sds = signalDatastore(outputLocation,IncludeSubfolders=true, ...
    SignalVariableNames=["spec","label"],ReadOutputOrientation="row");

tdsVal = transform(adsVal,@(x){HelperMelSpectrograms(x,fs, ...
    WindowLength=windowLength, ...
    HopLength=samplesPerHop, ...
    NumBands=numBands, ...
    FFTLength=fftLength)});
xVal = readall(tdsVal,UseParallel=canUseParallelPool);
xVal = cat(4,xVal{:});
yVal = adsVal.Labels;

% Define CNN architecture
imgSize = [size(xVal,1),size(xVal,2),size(xVal,3)];
numF = 32;
layers = [ ...
    imageInputLayer(imgSize)

    batchNormalizationLayer

    convolution2dLayer(3,numF,Padding="same")
    batchNormalizationLayer
    reluLayer
    convolution2dLayer(3,numF,Padding="same")
    batchNormalizationLayer
    reluLayer 

    maxPooling2dLayer(3,Stride=2,Padding="same")

    convolution2dLayer(3,2*numF,Padding="same")
    batchNormalizationLayer
    reluLayer
    convolution2dLayer(3,2*numF,Padding="same")
    batchNormalizationLayer
    reluLayer

    maxPooling2dLayer(3,Stride=2,Padding="same")

    convolution2dLayer(3,4*numF,Padding="same")
    batchNormalizationLayer
    reluLayer
    convolution2dLayer(3,4*numF,Padding="same")
    batchNormalizationLayer
    reluLayer

    maxPooling2dLayer(3,Stride=2,Padding="same")

    convolution2dLayer(3,8*numF,Padding="same")
    batchNormalizationLayer
    reluLayer
    convolution2dLayer(3,8*numF,Padding="same")
    batchNormalizationLayer
    reluLayer

    globalAveragePooling2dLayer

    dropoutLayer(0.5)

    fullyConnectedLayer(2)
    softmaxLayer];

% Define training
miniBatchSize = 32;
tuneme = 128;
lr = 0.05*miniBatchSize/tuneme;

options = trainingOptions( ...
    "adam", ...
    ...
    InitialLearnRate=1e-4, ...
    MaxEpochs=20, ...
    LearnRateSchedule="piecewise", ...
    LearnRateDropFactor=0.1, ...
    LearnRateDropPeriod=15, ...
    MiniBatchSize=miniBatchSize, ...
    Shuffle="every-epoch", ...
    Plots="training-progress", ...
    Metrics="accuracy", ...
    Verbose=false, ...
    ...
    ValidationData={xVal,yVal}, ...
    ValidationFrequency=ceil(numel(adsTrain.Files)/miniBatchSize),...
    ...
    ExecutionEnvironment="gpu");

netTrained = trainnet(sds,layers,"crossentropy",options);