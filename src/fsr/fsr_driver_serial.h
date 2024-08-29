/**
 * @brief		functions for interfacing with usb connection for the fsr
 * 
 * @author		John
 * 
 */

#ifndef FSR_DRIVER_SERIAL_H
#define FSR_DRIVER_SERIAL_H

#include "SerialPort.hpp"
#include <cstdint>
#include <string>
#include <array>
#include <vector>

class fsr_driver_serial{
  private:
    static constexpr const float BAUDRATE = 115200; 
    static const std::string DEVICE;

    uint16_t headerFrame;
    std::vector<uint16_t> databuf; 

    uint16_t expected_headerFrame; // = 0xAA55 

    static const int USB_PACKET_LENGTH = 8; // 2 bytes header + 3 * 2 bytes for ADC values = 8 bytes
    uint8_t usb_receive_buf[USB_PACKET_LENGTH];

    bool isAligned = false;

  public:

    fsr_driver_serial();

    /**
     * @brief		Search for usb port and open device
     */
    void loadCell_init(mn::CppLinuxSerial::SerialPort* port);

    /**
     * @brief		Read from usb device
     */
    int loadCell_read(mn::CppLinuxSerial::SerialPort* port);

    /**
     * @brief		Decode the buffer from the usb device
     */
    void fsr_decodeBuffer(uint8_t* buf);

    std::vector<uint16_t>& get_dataBuf();

    uint16_t reverseUint16Bytes(uint16_t value);

};


#endif // FSR_DRIVER_SERIAL_H