# processing/trilateration.py
import logging
from scipy.signal import correlate
from config.settings import *

def determine_reference_mic(recordings_list):
    """
    Determines the reference mic using whole-sample cross-correlation.
    """
    logging.info("[Trilateration] Determining reference mic based on cross-correlation...")
    scores = {}
    for i, mic in enumerate(MIC_ORDER):
        score = 0.0
        for j, other_mic in enumerate(MIC_ORDER):
            if i == j:
                continue
            ref_signal = recordings_list[i]
            other_signal = recordings_list[j]
            correlation = correlate(ref_signal, other_signal, mode='full')
            lags = np.arange(-len(ref_signal) + 1, len(ref_signal)) / SAMPLE_RATE
            peak_index = np.argmax(np.abs(correlation))
            lag = lags[peak_index]
            score += lag
            logging.info(f"[Trilateration] Cross-correlation: Mic {mic} vs Mic {other_mic}: lag = {lag:.6f} s")
        scores[mic] = score
        logging.info(f"[Trilateration] Total score for Mic {mic}: {score:.6f} s")
    reference_mic = max(scores, key=scores.get)
    logging.info(f"[Trilateration] Reference mic chosen: Mic {reference_mic}")
    reordered_mics = [reference_mic] + [m for m in MIC_ORDER if m != reference_mic]
    return reference_mic, reordered_mics

def cross_correlate(recordings_ordered, reordered_mics):
    """
    Computes time delays (lags) between the reference mic and the other mics.
    """
    logging.info("[Trilateration] Computing time lags relative to the reference mic...")
    ref_signal = recordings_ordered[0]
    time_lags = {}
    for i in range(1, len(recordings_ordered)):
        mic = reordered_mics[i]
        mic_signal = recordings_ordered[i]
        correlation = correlate(ref_signal, mic_signal, mode='full')
        lags = np.arange(-len(ref_signal) + 1, len(ref_signal)) / SAMPLE_RATE
        peak_index = np.argmax(np.abs(correlation))
        time_lag = lags[peak_index]
        time_lags[mic] = time_lag
        logging.info(f"[Trilateration] Time lag: Ref Mic {reordered_mics[0]} vs Mic {mic}: {time_lag:.6f} s")
    return time_lags

def localize_source(time_lags, reordered_mics):
    """
    Converts time lags to distances and applies trilateration.
    """
    logging.info("[Trilateration] Performing trilateration to locate sound source...")
    if len(reordered_mics) < 3:
        logging.info("[Trilateration] Not enough microphones for trilateration.")
        return np.array([None, None]), 0, 0
    # Convert time lags to distances
    r1 = time_lags.get(reordered_mics[1], 0) * SPEED_OF_SOUND
    r2 = time_lags.get(reordered_mics[2], 0) * SPEED_OF_SOUND
    r3 = 0  # Reference mic
    logging.info(f"[Trilateration] Distances: r1={r1:.3f} m, r2={r2:.3f} m")
    tower1 = MIC_POSITIONS[reordered_mics[0]]
    tower2 = MIC_POSITIONS[reordered_mics[1]]
    tower3 = MIC_POSITIONS[reordered_mics[2]]
    A = 2 * (tower2 - tower1)
    B = 2 * (tower3 - tower1)
    C = r1**2 - r2**2 - np.dot(tower1, tower1) + np.dot(tower2, tower2)
    D = r1**2 - r3**2 - np.dot(tower1, tower1) + np.dot(tower3, tower3)
    A_matrix = np.array([A, B]).T
    b_vector = np.array([C, D])
    try:
        estimated_position = np.linalg.solve(A_matrix, b_vector)
        logging.info(f"[Trilateration] Estimated position: x = {estimated_position[0]:.3f}, y = {estimated_position[1]:.3f}")
    except np.linalg.LinAlgError:
        logging.info("[Trilateration] Error: Singular matrix during trilateration.")
        estimated_position = np.array([None, None])
    return estimated_position, r1, r2
