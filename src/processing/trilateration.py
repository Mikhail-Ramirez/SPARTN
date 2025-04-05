# processing/trilateration.py
import logging
import numpy as np
from scipy.signal import correlate
from config.settings import *

def analyze_microphones(recordings_list: list[np.ndarray]) -> tuple[str, list[str], dict[str, float]]:
    """
    Computes cross-correlation lags for each pair of microphones only once,
    then determines the reference mic based on the sum of lags and computes
    the time lags relative to that reference mic.
    
    Returns:
        reference_mic: The chosen reference microphone.
        reordered_mics: List of mics with the reference mic first.
        time_lags: Dictionary mapping each non-reference mic to its time lag.
    """
    logging.info("[Trilateration] Analyzing microphones with a single set of cross-correlation computations...")
    n = len(MIC_ORDER)
    correlation_results = {}
    
    # Compute cross-correlation for each unique mic pair
    for i in range(n):
        for j in range(i + 1, n):
            ref_signal = recordings_list[i]
            other_signal = recordings_list[j]
            correlation = correlate(ref_signal, other_signal, mode='full')
            lags = np.arange(-len(ref_signal) + 1, len(ref_signal)) / SAMPLE_RATE
            peak_index = np.argmax(np.abs(correlation))
            lag = lags[peak_index]
            correlation_results[(i, j)] = lag
            correlation_results[(j, i)] = -lag
            logging.info(f"[Trilateration] Cross-correlation: Mic {MIC_ORDER[i]} vs Mic {MIC_ORDER[j]}: lag = {lag:.6f} s")
    
    # Score each mic by summing lags with others
    scores = {}
    for i in range(n):
        score = sum(correlation_results[(i, j)] for j in range(n) if i != j)
        scores[MIC_ORDER[i]] = score
        logging.info(f"[Trilateration] Total score for Mic {MIC_ORDER[i]}: {score:.6f} s")
    
    # Select reference mic with highest total lag sum
    reference_mic = max(scores, key=scores.get)
    logging.info(f"[Trilateration] Reference mic chosen: Mic {reference_mic}")
    reordered_mics = [reference_mic] + [m for m in MIC_ORDER if m != reference_mic]
    
    ref_index = MIC_ORDER.index(reference_mic)
    time_lags = {
        mic: correlation_results[(ref_index, MIC_ORDER.index(mic))]
        for mic in MIC_ORDER if mic != reference_mic
    }
    for mic, lag in time_lags.items():
        logging.info(f"[Trilateration] Time lag: Ref Mic {reference_mic} vs Mic {mic}: {lag:.6f} s")
    
    return reference_mic, reordered_mics, time_lags


def localize_source(time_lags: dict[str, float], reordered_mics: list[str]) -> tuple[np.ndarray, float, float]:
    """
    Converts time lags to distances and applies trilateration.

    Returns:
        estimated_position: 2D position estimate (x, y) or (None, None) on error.
        dist_to_mic2, dist_to_mic3: Distances used in the trilateration calculation.
    """
    logging.info("[Trilateration] Performing trilateration to locate sound source...")

    if len(reordered_mics) < 3:
        logging.info("[Trilateration] Not enough microphones for trilateration.")
        return np.array([None, None]), 0.0, 0.0

    # Convert time lags to distances (meters)
    dist_to_mic2 = time_lags.get(reordered_mics[1], 0.0) * SPEED_OF_SOUND
    dist_to_mic3 = time_lags.get(reordered_mics[2], 0.0) * SPEED_OF_SOUND
    dist_to_ref = 0.0  # Distance to reference mic is zero

    logging.info(f"[Trilateration] Distances: d2={dist_to_mic2:.3f} m, d3={dist_to_mic3:.3f} m")

    tower1 = MIC_POSITIONS[reordered_mics[0]]
    tower2 = MIC_POSITIONS[reordered_mics[1]]
    tower3 = MIC_POSITIONS[reordered_mics[2]]

    A = 2 * (tower2 - tower1)
    B = 2 * (tower3 - tower1)
    C = dist_to_mic2**2 - dist_to_mic3**2 - np.dot(tower1, tower1) + np.dot(tower2, tower2)
    D = dist_to_mic2**2 - dist_to_ref**2 - np.dot(tower1, tower1) + np.dot(tower3, tower3)

    A_matrix = np.array([A, B]).T
    b_vector = np.array([C, D])

    try:
        estimated_position = np.linalg.solve(A_matrix, b_vector)
        logging.info(f"[Trilateration] Estimated position: x = {estimated_position[0]:.3f}, y = {estimated_position[1]:.3f}")
    except np.linalg.LinAlgError:
        logging.info("[Trilateration] Error: Singular matrix during trilateration.")