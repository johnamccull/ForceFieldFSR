#include "fsr_driver_serial.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <iomanip>

const std::string fsr_driver_serial::DEVICE = "/dev/ttyACM0";

/**
 * @brief		Configure USB port to read from FSR
 */
void fsr_driver_serial::loadCell_init(mn::CppLinuxSerial::SerialPort* port) {
    port->SetDevice(DEVICE);
    port->SetBaudRate(FSR_BAUDRATE);
    port->SetTimeout(0);  // Asynchronous mode
    port->Open();
}

/**
 * @brief		Read function that reads data from the USB device and decodes it
 * @return		Number of bytes read
 */
int fsr_driver_serial::loadCell_read(mn::CppLinuxSerial::SerialPort* port) {
    while (!isAligned) {
        port->Read(usb_receive_buf, FSR_PACKET_LENGTH);
        fsr_decodeBuffer(usb_receive_buf);

        if (headerFrame == EXPECTED_HEADER_FRAME) {
            isAligned = true;
        } else {
            port->Close();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            port->Open();
        }
    }

    port->Read(usb_receive_buf, FSR_PACKET_LENGTH);
    fsr_decodeBuffer(usb_receive_buf);
    return 0;
}

/**
 * @brief		Helper function to decode buffer sent by load cell
 * 
 * Decoding is done by memcopying the data to a struct.  This struct is
 * matched to the size and format of the expected data
 */
void fsr_driver_serial::fsr_decodeBuffer(const uint8_t* rcvbuf) {
    size_t offset = 0;

	// Print the raw buffer data
    /*std::cout << "Raw buffer data: ";
    for (size_t i = 0; i < FSR_PACKET_LENGTH; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rcvbuf[i]) << " ";
    }
    std::cout << std::dec << std::endl; // Reset to decimal*/

	// 1. Copy headerFrame (2 bytes)
    memcpy(&headerFrame, rcvbuf + offset, sizeof(headerFrame));
    headerFrame = reverseUint16Bytes(headerFrame);
    offset += sizeof(headerFrame);

	// 2. Copy databuf (6 bytes, 3 uint16_t elements)
	databuf.resize(3); 
    for (size_t i = 0; i < databuf.size(); ++i) {
        uint16_t tempValue;
        memcpy(&tempValue, rcvbuf + offset, sizeof(tempValue));
        databuf[i] = reverseUint16Bytes(tempValue);
        offset += sizeof(tempValue);
    }

	// Print decoded values in one line
    //std::cout << "Decoded headerFrame: 0x" << std::hex << std::setw(4) << std::setfill('0') << headerFrame << " | ";
	/*std::cout << "Decoded databuf values: ";
    for (const auto& value : databuf) {
        // Convert uint16_t to float (assuming raw data and no specific scaling is needed)
        float floatValue = static_cast<float>(value); // Replace this with actual conversion if needed
        std::cout << std::fixed << std::setprecision(2) << floatValue << " ";
    }
    std::cout << std::endl; // Reset to decimal
	*/

    if (headerFrame != EXPECTED_HEADER_FRAME) {
        isAligned = false;
    }
}

std::vector<uint16_t>& fsr_driver_serial::get_dataBuf(){
    return databuf;
}

uint16_t fsr_driver_serial::reverseUint16Bytes(uint16_t value) {
    return (value >> 8) | (value << 8);
}
