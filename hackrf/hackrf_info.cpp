#include <iostream>
#include <libhackrf/hackrf.h>

void check_hackrf_status(int status, const std::string& message) {
    if (status != HACKRF_SUCCESS) {
        std::cerr << "Error: " << message << " (" << hackrf_error_name((hackrf_error)status) << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
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

    // Read board information
    uint8_t board_id = 0;
    status = hackrf_board_id_read(device, &board_id);
    check_hackrf_status(status, "Failed to read board ID");
    std::cout << "Board ID: " << hackrf_board_id_name((hackrf_board_id)board_id) << std::endl;

    char version_str[256] = {0};
    status = hackrf_version_string_read(device, version_str, (uint8_t)(sizeof(version_str) - 1));
    check_hackrf_status(status, "Failed to read version string");
    std::cout << "Version: " << version_str << std::endl;

    read_partid_serialno_t part_id_serial;
    status = hackrf_board_partid_serialno_read(device, &part_id_serial);
    check_hackrf_status(status, "Failed to read part ID and serial number");

    std::cout << "Part ID: ";
    for (int i = 0; i < 2; i++) {
        printf("%08X", part_id_serial.part_id[i]);
    }
    std::cout << std::endl;

    std::cout << "Serial No: ";
    for (int i = 0; i < 4; i++) {
        printf("%08X", part_id_serial.serial_no[i]);
    }
    std::cout << std::endl;

    // Close the device
    hackrf_close(device);

    // Cleanup
    hackrf_exit();
    return 0;
}

