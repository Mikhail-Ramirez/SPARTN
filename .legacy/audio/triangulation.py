import numpy as np
import matplotlib.pyplot as plt

# Speed of sound in meters per second
speed_of_sound = 343

# Positions of the towers (in meters)
tower1 = np.array([0, 0]) # mic 4
tower2 = np.array([1, 0]) # mic 2 
tower3 = np.array([0, 1]) # mic 3

# Position of the sound source (in meters)
sound_source = np.array([0, -0.5])

# Step 1: Calculate distances from the sound source to each tower
distance_to_tower1 = np.linalg.norm(sound_source - tower1)
distance_to_tower2 = np.linalg.norm(sound_source - tower2)
distance_to_tower3 = np.linalg.norm(sound_source - tower3)

print(f"Distance to Tower 1: {distance_to_tower1:.2f} meters")
print(f"Distance to Tower 2: {distance_to_tower2:.2f} meters")
print(f"Distance to Tower 3: {distance_to_tower3:.2f} meters")

# Step 2: Calculate arrival times at each tower
arrival_time_tower1 =  distance_to_tower1 / 343
arrival_time_tower2 =  distance_to_tower2 / 343
arrival_time_tower3 =  distance_to_tower3 / 343


print(f"Arrival time at Tower 1: {arrival_time_tower1:.6f} seconds")
print(f"Arrival time at Tower 2: {arrival_time_tower2:.6f} seconds")
print(f"Arrival time at Tower 3: {arrival_time_tower3:.6f} seconds")

print(f"Time lag between Mic 4 (Ref) and Mic 2: {arrival_time_tower2 - arrival_time_tower1: .6f} seconds")
print(f"Time lag between Mic 4 (Ref) and Mic 3: {arrival_time_tower3 - arrival_time_tower1: .6f} seconds")



# Step 3: Use arrival times to estimate the location of the sound source
# We'll use trilateration to estimate the position of the sound source
# Trilateration is the process of determining absolute or relative locations of points by measurement of distances.

# Function to perform trilateration
def trilaterate(tower1, tower2, tower3, r1, r2, r3):
    A = 2 * (tower2 - tower1)
    B = 2 * (tower3 - tower1)
    C = r1**2 - r2**2 - np.dot(tower1, tower1) + np.dot(tower2, tower2)
    D = r1**2 - r3**2 - np.dot(tower1, tower1) + np.dot(tower3, tower3)
    
    # Solving the system of equations
    A_matrix = np.array([A, B]).T
    b_vector = np.array([C, D])
    
    # Solving for the position of the sound source
    estimated_position = np.linalg.solve(A_matrix, b_vector)
    return estimated_position

# Estimated distances based on arrival times
r1 = arrival_time_tower1 * speed_of_sound
r2 = arrival_time_tower2 * speed_of_sound
r3 = arrival_time_tower3 * speed_of_sound

# Estimate the position of the sound source
estimated_position = trilaterate(tower1, tower2, tower3, r1, r2, r3)

print(f"Estimated position of the sound source: ({estimated_position[0]:.2f}, {estimated_position[1]:.2f}) meters")
