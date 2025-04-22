#!/usr/bin/env python3
import sys
import os
import re
import ast
from collections import defaultdict
import statistics

def parse_log_file(filename):
    """
    Parse the log file to extract [RESULT] entries.
    Each [RESULT] line contains a dictionary with lag values keyed by a tuple (i, j).
    """
    result_pattern = re.compile(r'\[RESULT\]\s+(?P<dict>\{.*\})')
    
    results = []  # List of dictionaries for each iteration
    
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    # Iterate through lines and parse
    for i, line in enumerate(lines):
        result_match = result_pattern.search(line)
        if result_match:
            dict_str = result_match.group("dict")
            try:
                # Safely evaluate the dictionary literal
                corr_dict = ast.literal_eval(dict_str)
            except Exception as e:
                print(f"Error parsing dictionary on line {i+1}: {e}")
                continue
            results.append(corr_dict)
    return results

def compute_avg_lags(corr_dict):
    """
    For each microphone, compute the average lag from that mic to all other mics.
    The lag for mic i is computed as the mean of all values lag(i,j) where j != i.
    """
    mic_avgs = {}
    # Get unique microphone indices from the keys
    mics = set()
    for key in corr_dict.keys():
        mics.update(key)
    for mic in mics:
        lags = []
        for key, lag in corr_dict.items():
            if key[0] == mic and key[1] != mic:
                lags.append(lag)
        if lags:
            mic_avgs[mic] = sum(lags) / len(lags)
        else:
            mic_avgs[mic] = 0.0
    return mic_avgs

def analyze_results(results):
    """
    Analyze the results by aggregating the average lag for each microphone over all iterations.
    Computes the overall mean and standard deviation for each mic.
    """
    mic_avg_samples = defaultdict(list)
    
    for corr_dict in results:
        avg_lags = compute_avg_lags(corr_dict)
        for mic, avg in avg_lags.items():
            mic_avg_samples[mic].append(avg)
    
    summary = {}
    for mic, samples in mic_avg_samples.items():
        avg_overall = statistics.mean(samples)
        std_dev = statistics.stdev(samples) if len(samples) > 1 else 0.0
        summary[mic] = {
            "average_lag": avg_overall,
            "std_dev": std_dev,
            "num_samples": len(samples)
        }
    return summary

def generate_summary_text(summary, total_iterations):
    """
    Create a textual summary that contextualizes the average lag measurements for each microphone.
    """
    lines = []
    lines.append("SPARTN Pi System OS Lag Analysis Report\n")
    lines.append(f"Total iterations analyzed: {total_iterations}\n")
    lines.append("Microphone Average Lag Metrics:\n")
    for mic in sorted(summary.keys()):
        avg = summary[mic]["average_lag"]
        std = summary[mic]["std_dev"]
        nsamples = summary[mic]["num_samples"]
        lines.append(f"  - Microphone {mic}:\n")
        lines.append(f"      Average Lag       : {avg:.6f} s\n")
        lines.append(f"      Standard Deviation: {std:.6f} s\n")
        lines.append(f"      Iterations Sampled: {nsamples}\n")
    
    lines.append("\nInterpretation and Context:\n")
    lines.append("  The average lag for each microphone represents the mean time delay from that mic to all other mics.\n")
    lines.append("  A lower average lag suggests that the mic is more in sync with its counterparts, potentially due to fewer internal OS delays.\n")
    lines.append("  The standard deviation indicates the consistency of this delay measurement. A higher value points to greater jitter or inconsistency in OS scheduling.\n")
    lines.append("\n  In essence, these metrics reveal how each mic's timing performance compares across multiple iterations, giving insights into inherent system perturbations.\n")
    lines.append("  The numbers don’t lie—if one mic is lagging, it’s time to call it out!\n")
    
    return "\n".join(lines)

def main():
    if len(sys.argv) != 2:
        print("Usage: python analyze_os_lag.py <log_file>")
        sys.exit(1)
    
    input_filename = sys.argv[1]
    if not os.path.isfile(input_filename):
        print(f"Error: File '{input_filename}' not found.")
        sys.exit(1)
    
    results = parse_log_file(input_filename)
    if not results:
        print("No [RESULT] entries found in the log file. Exiting.")
        sys.exit(1)
    
    summary = analyze_results(results)
    report_text = generate_summary_text(summary, total_iterations=len(results))
    
    # Determine output filename: append '_analysis.txt' to the original filename (without extension)
    base, ext = os.path.splitext(input_filename)
    output_filename = f"{base}_analysis.txt"
    
    with open(output_filename, 'w') as outfile:
        outfile.write(report_text)
    
    print(f"Analysis complete. Report written to '{output_filename}'.")

if __name__ == '__main__':
    main()

