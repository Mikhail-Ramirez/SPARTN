# processing/trilateration.py
import logging
from scipy.signal import correlate
from config.settings import *


def get_shift_percentages(recordings_list): 
    """
    Returns an ordered list of shift percentage values, calculated from mic amplitudes
    relative to the scale of possible amplitude levels. 
    """
    import numpy as np
    import logging

    # Scale values, these represent the loudest value and quietest values that we expect 
    # Tested the system on 1 mic using this setup, results are decent. Further fine tuning 
    # needed for the values below: 
    max_amp = 0.9
    min_amp = 0.2
    diff = max_amp - min_amp
    shift_percent_values = [0.0, 0.0, 0.0, 0.0]

    # form the list of ordered mic amplitude shift values 
    for i, recording in enumerate(recordings_list):
        amp = np.max(np.abs(recording)) # Current max amplitude of this mic 
        logging.info(f"[Audio Analysis] Mic index {i}: max amplitude = {amp:.6f}")

        if amp > max_amp:
            shift = 1.0
        elif amp < min_amp: 
            shift = 0.0
        else: 
            shift = (amp - min_amp) / diff   # Represetns the percentage to shift the location towards this mic
        
        shift_percent_values[i] = shift

    return shift_percent_values

def get_loudest(recordings_list):
    """
    Returns the index of the microphone that recorded the loudest signal.
    
    Loudness is determined by the maximum absolute amplitude in the recording.
    
    Args:
        recordings_list (list of np.ndarray): List containing the audio recordings from each microphone.
    
    Returns:
        int: Index of the microphone with the highest peak amplitude.
    """
    import numpy as np
    import logging

    max_amplitude = -1  # Start with -1 to ensure any valid amplitude is higher
    loudest_index = -1

    for i, recording in enumerate(recordings_list):
        # Compute the maximum absolute amplitude of the current recording
        current_max = np.max(np.abs(recording))
        logging.info(f"[Audio Analysis] Mic index {i}: max amplitude = {current_max:.6f}")
        
        if current_max > max_amplitude:
            max_amplitude = current_max
            loudest_index = i

    logging.info(f"[Audio Analysis] Loudest mic index: {loudest_index} with amplitude: {max_amplitude:.6f}")
    return loudest_index

def analyze_microphones(recordings_list):
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
    # Store cross-correlation lags for each pair (i, j)
    # For i < j, we'll compute once and then derive the opposite for j < i.
    correlation_results = {}
    
    # Compute correlation for each unique pair
    for i in range(n):
        for j in range(i + 1, n):
            ref_signal = recordings_list[i]
            other_signal = recordings_list[j]
            correlation = correlate(ref_signal, other_signal, mode='full')
            lags = np.arange(-len(ref_signal) + 1, len(ref_signal)) / SAMPLE_RATE
            peak_index = np.argmax(np.abs(correlation))
            lag = lags[peak_index]
            # Store lag for pair (i, j)
            correlation_results[(i, j)] = lag
            # For the opposite pair, the lag is the negative
            correlation_results[(j, i)] = -lag
            logging.info(f"[Trilateration] Cross-correlation: Mic {MIC_ORDER[i]} vs Mic {MIC_ORDER[j]}: lag = {lag:.6f} s")
    
    # Determine a score for each mic by summing its lags with every other mic.
    scores = {}
    for i in range(n):
        score = 0.0
        for j in range(n):
            if i == j:
                continue
            score += correlation_results[(i, j)]
        scores[MIC_ORDER[i]] = score
        logging.info(f"[Trilateration] Total score for Mic {MIC_ORDER[i]}: {score:.6f} s")
    
    # Choose the reference mic with the highest score.
    reference_mic = max(scores, key=scores.get)
    logging.info(f"[Trilateration] Reference mic chosen: Mic {reference_mic}")
    reordered_mics = [reference_mic] + [m for m in MIC_ORDER if m != reference_mic]
    
    # Compute time lags relative to the reference mic using the stored correlations.
    ref_index = MIC_ORDER.index(reference_mic)
    time_lags = {}
    for i in range(n):
        if i == ref_index:
            continue
        mic = MIC_ORDER[i]
        lag = correlation_results[(ref_index, i)]
        time_lags[mic] = lag
        logging.info(f"[Trilateration] Time lag: Ref Mic {reference_mic} vs Mic {mic}: {lag:.6f} s")
    
    return reference_mic, reordered_mics, time_lags


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
