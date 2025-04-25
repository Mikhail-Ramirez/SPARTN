function X = HelperMelSpectrograms(x,fs,options)

arguments
    x
    fs
    %options.WindowLength = 1024
    %options.HopLength = 512
    %options.NumBands = 128
    %options.FFTLength = 1024
    options.WindowLength = 2048;
    options.HopLength = 1024;
    options.NumBands = 128;
    options.FFTLength = 4096;
end

%ensure all samples 1 second in length
x1 = x(1:48000);

% Convert audio to a Python list (or NumPy array)
pyAudio = py.numpy.array(x1(:));  % Make sure it's 1D
pyFs = int32(fs);
% Import librosa
librosa = py.importlib.import_module('librosa');
np = py.importlib.import_module('numpy');
% Access the 'feature' submodule
feature = py.getattr(librosa, 'feature');
% Access the melspectrogram function
melspec_func = py.getattr(feature, 'melspectrogram');
% Call melspectrogram
mel_spec = melspec_func(pyargs('y', pyAudio, 'sr', pyFs, 'n_fft', int32(options.FFTLength), 'hop_length', int32(options.HopLength), 'win_length', int32(options.WindowLength), 'n_mels', int32(options.NumBands)));
% Convert the result back to MATLAB
mel_spec_matlab = double(np.array(mel_spec));
spec = mel_spec_matlab(:, 3:end);

% Reshape the array to dimensions(Number of bands)-by-(Number of hops)-by-
% (Number of channels)-by-(Number of segments). When you feed an image into
% a neural network, the first two dimensions are the height and width of
% the image, the third dimension is the channels, and the fourth dimension
% separates the individual images.
X = reshape(spec,size(spec,1),size(spec,2),size(x,2),[]);
end