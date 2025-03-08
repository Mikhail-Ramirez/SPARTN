import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from scipy.optimize import least_squares

# -------------------------------
# Constants and Microphone Setup
# -------------------------------
speed_of_sound = 343  # m/s

# Microphone positions (as provided)
#mic1 = np.array([0, 4.87])
#mic2 = np.array([0, 0])
#mic3 = np.array([0, 9.94])

mic1 = np.array([0, 1])
mic2 = np.array([0, 0])
mic3 = np.array([1, 0])
# Perturbation in seconds (±)
delta_t = 0.000598

# -------------------------------
# Nonlinear Trilateration Function
# -------------------------------
def nonlinear_trilaterate(r1, r2, r3, initial_guess):
    """
    Solve for the source position (x,y) using nonlinear least squares.
    The residuals are the differences between the measured distances and the
    distances computed from an estimated (x,y) to each microphone.
    """
    def residuals(pos):
        x, y = pos
        res1 = np.sqrt((x - mic1[0])**2 + (y - mic1[1])**2) - r1
        res2 = np.sqrt((x - mic2[0])**2 + (y - mic2[1])**2) - r2
        res3 = np.sqrt((x - mic3[0])**2 + (y - mic3[1])**2) - r3
        return [res1, res2, res3]
    
    # Use least_squares with the Levenberg-Marquardt method.
    sol = least_squares(residuals, initial_guess, method='lm')
    return sol.x

# -------------------------------
# Generate True Source Positions
# -------------------------------
# 1000 source positions along the line from (1,1) to (100,100)
points = np.linspace(0, 5, 1000)
true_sources = np.column_stack((points, points))  # shape (1000, 2)

# Preallocate arrays for:
# - Centroid (average of the 7 estimated positions)
# - Confidence radius (max spread of the 7 estimates)
# - Centroid error (distance between centroid and true source)
centroids = np.zeros((len(true_sources), 2))
confidence_radii = np.zeros(len(true_sources))
centroid_errors = np.zeros(len(true_sources))

# -------------------------------
# Main Loop: Compute Estimates & Confidence Circle for Each Source
# -------------------------------
for i, source in enumerate(true_sources):
    # Compute true distances from the source to each mic.
    d1 = np.linalg.norm(source - mic1)
    d2 = np.linalg.norm(source - mic2)
    d3 = np.linalg.norm(source - mic3)
    
    # Compute the "perfect" arrival times (seconds)
    t1 = d1 / speed_of_sound
    t2 = d2 / speed_of_sound
    t3 = d3 / speed_of_sound
    
    # Define the 7 scenarios (arrival times)
    scenarios = [
        (t1, t2, t3),                  # Perfect
        (t1 + delta_t, t2, t3),          # mic1: +delta
        (t1 - delta_t, t2, t3),          # mic1: -delta
        (t1, t2 + delta_t, t3),          # mic2: +delta
        (t1, t2 - delta_t, t3),          # mic2: -delta
        (t1, t2, t3 + delta_t),          # mic3: +delta
        (t1, t2, t3 - delta_t)           # mic3: -delta
    ]
    
    estimates = []  # List to hold the 7 estimated positions for this source
    for (pt1, pt2, pt3) in scenarios:
        # Convert arrival times to distances.
        r1 = pt1 * speed_of_sound
        r2 = pt2 * speed_of_sound
        r3 = pt3 * speed_of_sound
        
        # Use the nonlinear solver with the true source as the initial guess.
        est_pos = nonlinear_trilaterate(r1, r2, r3, initial_guess=source)
        estimates.append(est_pos)
    
    estimates = np.array(estimates)  # shape (7, 2)
    
    # Compute the centroid (average of the 7 estimates)
    centroid = np.mean(estimates, axis=0)
    centroids[i, :] = centroid
    
    # Compute the confidence radius: maximum distance from the centroid among the estimates
    dists_from_centroid = np.linalg.norm(estimates - centroid, axis=1)
    confidence_radii[i] = np.max(dists_from_centroid)
    
    # Compute the centroid error (distance between the centroid and the true source)
    centroid_errors[i] = np.linalg.norm(centroid - source)

# Compute the true source distance (from the origin) for plotting.
true_source_distances = np.linalg.norm(true_sources, axis=1)

# -------------------------------
# Plotting
# -------------------------------
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))

# Plot 1: Error Metrics vs. True Source Distance.
ax1.plot(true_source_distances, confidence_radii, label='Confidence Circle Radius', color='C1')
ax1.plot(true_source_distances, centroid_errors, label='Centroid Error', color='C2', linestyle='--')
ax1.set_xlabel("True Source Distance from Origin (m)")
ax1.set_ylabel("Error (m)")
ax1.set_title("Trilateration Error vs. Source Distance")
ax1.legend()
ax1.grid(True)

# Plot 2: 2D Map with Sample Confidence Circles.
ax2.set_title("Sample Confidence Circles along the Source Path")
ax2.set_xlabel("X position (m)")
ax2.set_ylabel("Y position (m)")
ax2.grid(True)
ax2.plot(true_sources[:, 0], true_sources[:, 1], 'k-', lw=0.5, label='True Path')

# For clarity, plot every 100th point's confidence circle.
for idx in range(0, len(true_sources), 100):
    circ = Circle(centroids[idx], confidence_radii[idx], edgecolor='red',
                  facecolor='none', alpha=0.6,
                  label='Confidence Circle' if idx == 0 else None)
    ax2.add_patch(circ)
    # Mark the centroid and true source.
    ax2.plot(centroids[idx, 0], centroids[idx, 1], 'bo', markersize=4,
             label='Estimated Centroid' if idx == 0 else None)
    ax2.plot(true_sources[idx, 0], true_sources[idx, 1], 'gx', markersize=6,
             label='True Source' if idx == 0 else None)

ax2.legend()

plt.suptitle("Trilateration Accuracy & Confidence Circles\n(±{:.6f} s Perturbation per Mic)".format(delta_t), fontsize=14)
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.savefig('1m_trilateration_accuracy.png', dpi=300)
print("Plot saved as '1m_trilateration_accuracy.png'.")

# -------------------------------
# Write Summary Error Metrics to a Text File
# -------------------------------
summary_filename = "1m_trilateration_error_summary.txt"
with open(summary_filename, "w") as f:
    f.write("Source_Distance(m), Centroid_Error(m), Confidence_Radius(m)\n")
    for d, cent_err, conf_rad in zip(true_source_distances, centroid_errors, confidence_radii):
        f.write(f"{d:.6f}, {cent_err:.6f}, {conf_rad:.6f}\n")
print(f"Summary error metrics written to '{summary_filename}'.")

plt.show()

