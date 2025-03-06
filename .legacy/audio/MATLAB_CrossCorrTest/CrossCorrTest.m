[x,fs]=audioread("LaptopRec_Stereo.wav");
[y,fs]=audioread("PhoneRec_Stereo.wav");
lx = length(x);
ly = length(y);
samples = 1:min(lx,ly);
 subplot(3,1,1), plot (x(samples));
 subplot(3,1,2), plot (y(samples));
 [C1, lag1] = xcorr(x(samples),y(samples));
  subplot(3,1,3), plot(lag1/fs,C1);
  ylabel("Amplitude"); grid on
   title("Cross-correlation ")