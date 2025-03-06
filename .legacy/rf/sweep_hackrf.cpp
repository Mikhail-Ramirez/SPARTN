#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <libhackrf/hackrf.h>
#include <unistd.h>


void check_hackrf_status(int status, const std::string& message) {
    if (status != HACKRF_SUCCESS) {
        std::cerr << "Error: " << message << " (" << hackrf_error_name((hackrf_error)status) << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string generate_timestamp() {
    time_t now = time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", localtime(&now));
    return std::string(buf);
}

void write_sweep_data(const std::string& filename, const std::vector<std::string>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& line : data) {
        file << line << std::endl;
    }

    file.close();
    std::cout << "Sweep data written to: " << filename << std::endl;
}

int main() {
    hackrf_device* device = nullptr;

    // Initialize HackRF library
    int status = hackrf_init();
    check_hackrf_status(status, "Failed to initialize HackRF");

    // Open the first available HackRF device
    status = hackrf_open(&device);
    check_hackrf_status(status, "Failed to open HackRF device");

    std::cout << "HackRF device successfully opened." << std::endl;

    // Set frequency ranges for sweeping
    const std::vector<std::pair<uint64_t, uint64_t>> frequency_ranges = {
        { 900000000,  915000000},   // 900 MHz range
        {2400000000, 2500000000}, // 2.4 GHz range
        {4800000000, 4900000000}  // 4.8 GHz range
    };

    std::vector<std::string> sweep_data;
    std::cout << "Starting RF sweep..." << std::endl;

    for (const auto& range : frequency_ranges) {
        uint64_t start_freq = range.first;
        uint64_t end_freq = range.second;
        uint32_t step_size = 1000000; // 1 MHz step size
        std::cout << start_freq << "\n";

        for (uint64_t freq = start_freq; freq <= end_freq; freq += step_size) {
            int gain = 40; // Example gain value
            status = hackrf_set_freq(device, freq);
            check_hackrf_status(status, "Failed to set frequency");

            status = hackrf_set_lna_gain(device, gain);
            check_hackrf_status(status, "Failed to set LNA gain");

            std::ostringstream data_line;
            if (freq > 1000000000) { //If the frequency is greater than 1 GHz, use 1 GHz formatting.
                data_line << "Frequency: " << freq / 1000000000.0 << " GHz, Gain: " << gain;
            }
            else { //Otherwise, use MHz formatting
                data_line << "Frequency: " << freq / 1000000 << " MHz, Gain: " << gain;
            }

            sweep_data.push_back(data_line.str());
            std::cout << data_line.str() << std::endl;

            // Simulate data acquisition (replace with actual signal processing)
            usleep(50000); // 50ms delay to simulate sweep
        }
    }

    std::string timestamp = generate_timestamp();
    std::string output_file = "sweep_data_" + timestamp + ".txt";

    write_sweep_data(output_file, sweep_data);

    // Close the device
    hackrf_close(device);

    // Cleanup
    hackrf_exit();
    std::cout << "HackRF device closed. Sweep complete." << std::endl;

    return 0;
}

