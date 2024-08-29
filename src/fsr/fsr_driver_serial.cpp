#include "fsr_driver_serial.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <algorithm> 
#include <bitset>
#include <climits>
#include <numeric> 
#include <iomanip>

const std::string fsr_driver_serial::DEVICE = "/dev/ttyACM0";

fsr_driver_serial::fsr_driver_serial(){
	expected_headerFrame = 0xAA55;
}

/**
 * @brief		Configure USB port to read from FSR
 */
void fsr_driver_serial::loadCell_init(mn::CppLinuxSerial::SerialPort* port) {
    port->SetDevice(DEVICE);
    port->SetBaudRate(BAUDRATE);
    port->SetTimeout(0);  // Set to asynchronous mode
    port->Open();
}

/**
 * @brief		Read function that reads data from the USB device and decodes it
 * @return		Number of bytes read
 */
int fsr_driver_serial::loadCell_read(mn::CppLinuxSerial::SerialPort* port) {

	static int cnt = 0;
    // Aligment sequence
	while (!isAligned) {
		port->Read(usb_receive_buf, USB_PACKET_LENGTH);
		//std::cout << "read successful\n";
		fsr_decodeBuffer(usb_receive_buf);

		std::cout << "Misaligned data, realigning..." << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		//if ((cnt>20)) {
		//	cnt = 0;
		//	std::cout << "still trying to align\n";
		//}

		//std::cout << "tick:" <<_loadData->osTime_ms <<", error:"<< test_cnt 
		//	<< ", state:"<< (int) _loadData->gpioState << ", data[0]:" << _loadData->databuf[0] <<std::endl;

		std::cout << "headerFrame: " << std::hex << headerFrame << std::endl;
		if ((float)headerFrame == expected_headerFrame) {
			isAligned = true;
			std::cout << "Load cell data successfully aligned." << std::endl;
		} else {
			port->Close();
			sleep(0.02);

			port->Open();  
			sleep(0.02);
		}

		cnt++;
	}

	port->Read(usb_receive_buf, USB_PACKET_LENGTH);
	//std::cout << "read successful\n";
	fsr_decodeBuffer(usb_receive_buf);
	//std::cout << "decoded\n";

    return 0;
}

/**
 * @brief		Helper function to decode buffer sent by load cell
 * 
 * Decoding is done by memcopying the data to a struct.  This struct is
 * matched to the size and format of the expected data
 */
void fsr_driver_serial::fsr_decodeBuffer(uint8_t* rcvbuf){
	size_t offset = 0;

	// Print the raw buffer data
    /*std::cout << "Raw buffer data: ";
    for (size_t i = 0; i < USB_PACKET_LENGTH; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rcvbuf[i]) << " ";
    }
    std::cout << std::dec << std::endl; // Reset to decimal*/

    // 1. Copy headerFrame (2 bytes)
    memcpy(&headerFrame, rcvbuf + offset, sizeof(headerFrame));
    offset += sizeof(headerFrame);

	headerFrame = reverseUint16Bytes(headerFrame);
    
	// 2. Copy databuf (6 bytes, 3 uint16_t elements)
    databuf.resize(3); // Ensure the vector has enough space
    for (size_t i = 0; i < databuf.size(); ++i) {
        uint16_t tempValue;
        memcpy(&tempValue, rcvbuf + offset, sizeof(tempValue));
        databuf[i] = reverseUint16Bytes(tempValue);
        offset += sizeof(uint16_t);
    }


	// Print the decoded values
    /*std::cout << "Decoded headerFrame: 0x" << std::hex << std::setw(4) << std::setfill('0') << headerFrame << std::dec << std::endl;
    std::cout << "Decoded databuf values: ";
    for (const auto& value : databuf) {
        std::cout << value << " ";
    }
    std::cout << std::endl;*/

	if (headerFrame != expected_headerFrame) {
		isAligned = false;
		std::cout << "Load cell IS NOT ALIGNED." << std::endl;
	}
}

std::vector<uint16_t>& fsr_driver_serial::get_dataBuf() {
    return databuf;
}

uint16_t fsr_driver_serial::reverseUint16Bytes(uint16_t value) {
    uint8_t bytes[2];
    // Copy the uint16_t into a byte array
    memcpy(bytes, &value, sizeof(value));

    // Reverse the byte array
    std::reverse(bytes, bytes + sizeof(bytes));

    uint16_t reversedValue;
    // Copy the reversed byte array back into a uint16_t
    memcpy(&reversedValue, bytes, sizeof(reversedValue));

    return reversedValue;
}