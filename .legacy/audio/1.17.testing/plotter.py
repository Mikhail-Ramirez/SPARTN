import csv
import matplotlib.pyplot as plt
import numpy as np

# Verified microphone positions (must match the logging script)
MIC_POSITIONS = {
    2: np.array([0, 0]),
    3: np.array([3.6576, 3.6576]),
    4: np.array([7.3152, 0])
}

CSV_FILE = "sound_source_log.csv"

def read_all_measurements(csv_file):
    measurements = []
    with open(csv_file, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            try:
                # Attempt to convert the estimated positions to floats
                est_x = float(row["estimated_x"])
                est_y = float(row["estimated_y"])
                # Only include measurements within the specified bounds
                if 0 < est_x < 30 and -5 < est_y < 30:
                    measurements.append(row)
            except (ValueError, TypeError):
                continue
    return measurements

def plot_all_measurements(measurements):
    fig, ax = plt.subplots(figsize=(8, 8))
    
    # Plot all tower (mic) positions
    for mic_id, pos in MIC_POSITIONS.items():
        ax.scatter(pos[0], pos[1], s=100, label=f"Mic {mic_id}")
        ax.annotate(f"Mic {mic_id}", (pos[0], pos[1]), textcoords="offset points", xytext=(-10, -10))
    
    # Plot all estimated sound source positions from the measurements
    xs, ys = [], []
    for meas in measurements:
        try:
            xs.append(float(meas["estimated_x"]))
            ys.append(float(meas["estimated_y"]))
        except (ValueError, TypeError):
            continue
    ax.scatter(xs, ys, color='magenta', marker='x', s=50, label="Estimated Sound Source")
    
    # For the latest measurement, plot the circles used in trilateration (if valid)
    if measurements:
        latest = measurements[-1]
        try:
            mic_order_str = latest["mic_order"]  # e.g., "4-2-3"
            mic_order = [int(x) for x in mic_order_str.split('-')]
            r1 = float(latest["r1"])
            r2 = float(latest["r2"])
            
            # Retrieve tower positions according to the ordering in the latest measurement
            tower1 = MIC_POSITIONS[mic_order[0]]
            tower2 = MIC_POSITIONS[mic_order[1]]
            tower3 = MIC_POSITIONS[mic_order[2]]
            
            # Draw circles (using absolute distance values)
            circle2 = plt.Circle((tower2[0], tower2[1]), abs(r1), color='green', fill=False,
                                 linestyle='--', alpha=0.5, label=f"Dist from Mic {mic_order[1]}: {abs(r1):.2f} m")
            circle3 = plt.Circle((tower3[0], tower3[1]), abs(r2), color='blue', fill=False,
                                 linestyle='--', alpha=0.5, label=f"Dist from Mic {mic_order[2]}: {abs(r2):.2f} m")
            ax.add_artist(circle2)
            ax.add_artist(circle3)
        except Exception as e:
            print(f"Error plotting circles from latest measurement: {e}")
    
    ax.set_xlim(0, 30)
    ax.set_ylim(-5, 30)
    ax.set_xlabel("X Position (m)")
    ax.set_ylabel("Y Position (m)")
    ax.set_title("Trilateration Predictions")
    ax.legend()
    plt.tight_layout()
    plt.savefig("trilateration_plot_all.png")
    print("Plot saved to trilateration_plot_all.png")
    plt.show()

def main():
    measurements = read_all_measurements(CSV_FILE)
    if not measurements:
        print("No valid measurements found in the CSV log.")
        return
    plot_all_measurements(measurements)

if __name__ == "__main__":
    main()

