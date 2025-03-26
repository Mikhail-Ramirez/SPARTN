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


int decibelRead = 0;

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

void readRange(std::pair<int, int>, int, std::vector<std::string> &, hackrf_device*);

// Callback for data receive
int rx_callback(hackrf_transfer* transfer)
{
    // Write raw samples to output file
    //if (fp != NULL)
    //    fwrite(transfer->buffer, transfer->valid_length, 1, fp);

    int newReading = int(*(transfer->buffer));

    if (newReading > decibelRead) {
        decibelRead = int(*(transfer->buffer));
    }

    //fwrite(transfer->buffer, transfer->valid_length, 1, fp);

    // std::cout << (uintptr_t) transfer->buffer << std::endl;

    // To not capture too much traffic
    // usleep(50000);

    // Debugging recv
    // fprintf(stderr, "Valid length: %i\n", transfer->valid_length);
    // fprintf(stderr, "frequency: %llu\n", frequency);

    return 0;
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
    const std::vector<std::pair<int, int>> frequency_ranges = {
        { 900,  990},   // 900 MHz range
        {2400, 2500}, // 2.4 GHz range
        {4800, 4900}  // 4.8 GHz range
    };

    std::vector<std::string> sweep_data;
    std::cout << "Starting RF sweep..." << std::endl;

    //for (const auto& range : frequency_ranges) {
    //    readRange(range, status, sweep_data, device);
    //}

    readRange({ 914,  918 }, status, sweep_data, device);

    // Close the device
    hackrf_close(device);

    // Cleanup
    hackrf_exit();
    std::cout << "HackRF device closed. Sweep complete." << std::endl;

    return 0;
}

void readRange(std::pair<int, int> range, int status, std::vector<std::string>& sweep_data, hackrf_device* device) {
    int LNA = 40;
    int VGA = 62;

    uint64_t start_freq = range.first * 1000000.0;
    uint64_t end_freq = range.second * 1000000.0;
    uint32_t step_size = 1000000.0; // 1 MHz step size
    std::cout << start_freq << "\n";

    for (uint64_t freq = start_freq; freq <= end_freq; freq += step_size) {
        status = hackrf_set_freq(device, freq);
        check_hackrf_status(status, "Failed to set frequency");

        status = hackrf_set_lna_gain(device, LNA);
        check_hackrf_status(status, "Failed to set LNA gain");

        status = hackrf_set_vga_gain(device, VGA);
        check_hackrf_status(status, "Failed to set VGA gain");

        status |= hackrf_start_rx(device, rx_callback, NULL);
        check_hackrf_status(status, "rx_callback failed or HackRF failed to start listening.");

        usleep(50000);

        status = hackrf_stop_rx(device);
        check_hackrf_status(status, "Failed to stop!");

        std::ostringstream data_line;
        //data_line << "Frequency: " << freq / 1000000000.0 << " GHz, dB: ";
        //data_line << decibelRead;
        //sweep_data.push_back(data_line.str());

        //std::cout << data_line.str() << std::endl;

        std::cout << "Frequency: " << freq / 1000000000.0 << " GHz, dB: ";
        std::cout << decibelRead << std::endl;

        decibelRead = 0;
    }
}