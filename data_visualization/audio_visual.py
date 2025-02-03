import matplotlib.pyplot as plt

def read_audio_data(file_path):
    """Reads audio data from the given file and returns it as a list of floats."""
    audio_data = []
    with open(file_path, 'r') as file:
        for line in file:
            if "Audio Data [" in line:  # Start reading data after this line
                continue
            if "]" in line:  # Stop reading data at the closing bracket
                break
            # Split the line into individual values and convert them to float
            audio_data.extend(map(float, line.strip().split(", ")))
    return audio_data

def plot_audio_data(camera1_data, camera2_data, sample_rate):
    """Plots the audio data for both cameras."""
    # Generate time axis based on the sample rate
    time_camera1 = [i / sample_rate for i in range(len(camera1_data))]
    time_camera2 = [i / sample_rate for i in range(len(camera2_data))]

    # Plot Camera 1 data
    plt.figure(figsize=(12, 6))
    plt.plot(time_camera1, camera1_data, label="Camera 1", alpha=0.8)

    # Plot Camera 2 data
    plt.plot(time_camera2, camera2_data, label="Camera 2", alpha=0.8)

    # Add labels and title
    plt.title("Audio Data Visualization")
    plt.xlabel("Time (seconds)")
    plt.ylabel("Amplitude")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Save and show the plot
    plt.savefig("audio_data_visualization.png")
    plt.show()

if __name__ == "__main__":
    # File paths
    camera1_file = "../audio/camera1_audio_data.txt"
    camera2_file = "../audio/camera2_audio_data.txt"

    # Read data
    camera1_data = read_audio_data(camera1_file)
    camera2_data = read_audio_data(camera2_file)

    # Default sample rate for the microphones
    sample_rate = 44100  # Hz

    # Plot the audio data
    plot_audio_data(camera1_data, camera2_data, sample_rate)

