% Parameters
fs = 44100; % Sampling frequency in Hz
duration = 0.05; % Duration of the sound in seconds
f = 440; % Frequency of the square wave in Hz (A4 note)

% Time vector
t = 0:1/fs:duration;

% Generate square wave
squareWave = square(2 * pi * f * t);

% Play the sound
while true
    sound(squareWave, fs);
    pause(0.1);
end