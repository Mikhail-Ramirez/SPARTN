function myCustomWriter(spec,writeInfo,~)
% myCustomWriter(spec,writeInfo,~) writes an auditory spectrogram/label
% pair to MAT files.

spec = spec{1,1};
filename = strrep(writeInfo.SuggestedOutputName,".wav",".mat");
label = writeInfo.ReadInfo.Label;
save(filename,"label","spec");

end