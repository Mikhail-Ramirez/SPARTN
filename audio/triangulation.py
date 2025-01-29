# dual_mic_tdoa_localization.py

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

# Speed of sound in meters per second
SPEED_OF_SOUND = 343

# Microphone positions (in meters)
mic_positions = {
    'Mic1': (0.0, 0.0),  # Tower1
    'Mic2': (1.0, 0.0),  # Tower2
    'Mic3': (0.0, 1.0)   # Tower3
}

# Given Sound Source Position (for validation)
source_position = (0.75, 1.0)

# Calculate distances from source to each mic
def calculate_distances(source, mics):
    distances = {}
    for mic, pos in mics.items():
        distances[mic] = np.sqrt((source[0] - pos[0])**2 + (source[1] - pos[1])**2)
    return distances

# Calculate TDoA between pairs
def calculate_tdoa(distances, mics, pair1, pair2):
    delta_d = distances[pair1] - distances[pair2]
    delta_t = delta_d / SPEED_OF_SOUND
    return delta_t

# Define the system of equations based on TDoA
def equations(vars, mics, delta_d21, delta_d31):
    x, y = vars
    # Distances from source to each mic
    d1 = np.sqrt((x - mics['Mic1'][0])**2 + (y - mics['Mic1'][1])**2)
    d2 = np.sqrt((x - mics['Mic2'][0])**2 + (y - mics['Mic2'][1])**2)
    d3 = np.sqrt((x - mics['Mic3'][0])**2 + (y - mics['Mic3'][1])**2)
    
    # Equations based on distance differences
    eq1 = d2 - d1 - delta_d21
    eq2 = d3 - d1 - delta_d31
    return (eq1, eq2)

def main():
    # Step 1: Calculate actual distances (for validation)
    actual_distances = calculate_distances(source_position, mic_positions)
    print("Actual Distances from Source to Mics:")
    for mic, dist in actual_distances.items():
        print(f"{mic}: {dist:.3f} meters")
    
    # Step 2: Calculate TDoA (distance differences)
    delta_d21 = actual_distances['Mic2'] - actual_distances['Mic1']
    delta_d31 = actual_distances['Mic3'] - actual_distances['Mic1']
    
    # Step 3: Convert TDoA to distance differences (if starting from TDoA)
    # Here, we have distance differences directly
    # If starting from TDoA, use: delta_d = delta_t * SPEED_OF_SOUND
    
    print(f"\nDistance Differences:")
    print(f"Delta D21 (Mic2 - Mic1): {delta_d21:.3f} meters")
    print(f"Delta D31 (Mic3 - Mic1): {delta_d31:.3f} meters")
    
    # Step 4: Initial guess for (x, y)
    initial_guess = (0.5, 0.5)
    
    # Step 5: Solve the equations
    solution = fsolve(equations, initial_guess, args=(mic_positions, delta_d21, delta_d31))
    estimated_x, estimated_y = solution
    print(f"\nEstimated Source Position: ({estimated_x:.3f}, {estimated_y:.3f}) meters")
    
    # Step 6: Plotting
    fig, ax = plt.subplots(figsize=(8, 8))
    
    # Plot microphones
    for mic, pos in mic_positions.items():
        ax.plot(pos[0], pos[1], 'bo')
        ax.text(pos[0]+0.02, pos[1]+0.02, mic)
    
    # Plot estimated source position
    ax.plot(estimated_x, estimated_y, 'ro', label='Estimated Source')
    ax.text(estimated_x+0.02, estimated_y+0.02, 'Source')
    
    # Plot actual source position
    ax.plot(source_position[0], source_position[1], 'gx', label='Actual Source')
    ax.text(source_position[0]+0.02, source_position[1]+0.02, 'Actual Source')
    
    # Draw circles representing distance differences
    # Circle for Mic1
    circle1 = plt.Circle(mic_positions['Mic1'], actual_distances['Mic1'], color='b', fill=False, linestyle='--', label='Mic1 Radius')
    ax.add_artist(circle1)
    
    # Circle for Mic2
    circle2 = plt.Circle(mic_positions['Mic2'], actual_distances['Mic2'], color='b', fill=False, linestyle='--', label='Mic2 Radius')
    ax.add_artist(circle2)
    
    # Circle for Mic3
    circle3 = plt.Circle(mic_positions['Mic3'], actual_distances['Mic3'], color='b', fill=False, linestyle='--', label='Mic3 Radius')
    ax.add_artist(circle3)
    
    # Setting plot limits
    ax.set_xlim(-0.5, 1.5)
    ax.set_ylim(-0.5, 1.5)
    
    # Labels and title
    ax.set_xlabel('X Position (meters)')
    ax.set_ylabel('Y Position (meters)')
    ax.set_title('Sound Source Localization using TDoA')
    ax.legend()
    ax.grid(True)
    
    plt.show()

if __name__ == "__main__":
    main()

