function X = HelperMelSpectrograms(x,fs,options)

arguments
    x
    fs
    options.WindowLength = 1024
    options.HopLength = 512
    options.NumBands = 128
    options.FFTLength = 1024
end

%ensure all samples 1 second in length
x1 = x((1:44100),:);

% mid side encoding
x1 = [sum(x1,2),x1(:,1)-x1(:,2)];
x1 = x1./max(max(x1));

spec = melSpectrogram(x1,fs, ...
    Window=hamming(options.WindowLength,"periodic"), ...
    OverlapLength=options.WindowLength - options.HopLength, ...
    FFTLength=options.FFTLength, ...
    NumBands=options.NumBands);

% Reshape the array to dimensions(Number of bands)-by-(Number of hops)-by-
% (Number of channels)-by-(Number of segments). When you feed an image into
% a neural network, the first two dimensions are the height and width of
% the image, the third dimension is the channels, and the fourth dimension
% separates the individual images.
X = reshape(spec,size(spec,1),size(spec,2),size(x,2),[]);
end