#ifndef FSR_DRIVER_SERIAL_H
#define FSR_DRIVER_SERIAL_H

#include "SerialPort.hpp"
#include <cstdint>
#include <string>
#include <vector>

class fsr_driver_serial {
private:
    static constexpr float FSR_BAUDRATE = 115200; 
    static const std::string DEVICE;

    uint16_t headerFrame = 0;
    std::vector<uint16_t> databuf{3}; 

    static const uint16_t EXPECTED_HEADER_FRAME = 0xAA55;
    static const int FSR_PACKET_LENGTH = 8; 

    uint8_t usb_receive_buf[FSR_PACKET_LENGTH];
    bool isAligned = false;

    void fsr_decodeBuffer(const uint8_t* buf);

public:
    fsr_driver_serial() = default;

    void loadCell_init(mn::CppLinuxSerial::SerialPort* port);
    int loadCell_read(mn::CppLinuxSerial::SerialPort* port);
    std::vector<uint16_t>& get_dataBuf();

    static uint16_t reverseUint16Bytes(uint16_t value);
};

#endif // FSR_DRIVER_SERIAL_H
