# processing/trilateration.py

def determine_reference_mic(recordings_list):
    print("[Dummy] determine_reference_mic called.")
    # Return dummy reference mic and reordered mic list.
    return 2, [2, 3, 4]

def cross_correlate(recordings_ordered, reordered_mics):
    print("[Dummy] cross_correlate called.")
    # Return a dummy dictionary with time lags.
    return {2: 0.01, 3: 0.02}

def localize_source(time_lags, reordered_mics):
    print("[Dummy] localize_source called.")
    # Return dummy estimated position and dummy distances.
    return (1.0, 1.0), 0.1, 0.2

