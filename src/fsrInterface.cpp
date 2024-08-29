#include "fsrInterface.h"
#include <iostream>

fsrInterface::fsrInterface() : fsrDriver(), sensorData(3) {}

void fsrInterface::initialize() {
    // Initialize the serial port and FSR driver
    fsrDriver.loadCell_init(&serialPort);
}

void fsrInterface::updateSensorState() {
    // Read data from the FSR driver and update the sensorData buffer
    if (fsrDriver.loadCell_read(&serialPort) > 0) {
        sensorData = fsrDriver.get_dataBuf();
    }
}

float fsrInterface::getChannelReading(int channelNum) {
    // Ensure the channel number is valid (0-2 for the current setup)
    if (channelNum >= 0 && channelNum < static_cast<int>(sensorData.size())) {
        return static_cast<float>(sensorData[channelNum]);
    } else {
        std::cerr << "Invalid channel number: " << channelNum << std::endl;
        return -1.0f;
    }
}

void fsrInterface::shutdown() {
    // Close the serial port connection
    serialPort.Close();
}

float fsrInterface::getMeasurement(int unit) {
    // Return -1 as this function is not implemented
    return -1.0f;
}
