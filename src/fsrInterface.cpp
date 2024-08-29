#include "fsrInterface.h"
#include <iostream>

fsrInterface::fsrInterface() = default;

void fsrInterface::initialize() {
    fsrDriver.loadCell_init(&serialPort);
}

void fsrInterface::updateSensorState() {
    if (fsrDriver.loadCell_read(&serialPort) > 0) {
        sensorData = fsrDriver.get_dataBuf();
    }
}

float fsrInterface::getChannelReading(int channelNum) {
    if (channelNum >= 0 && channelNum < static_cast<int>(sensorData.size())) {
        return static_cast<float>(sensorData[channelNum]);
    } else {
        std::cerr << "Invalid channel number: " << channelNum << std::endl;
        return -1.0f;
    }
}

void fsrInterface::shutdown() {
    serialPort.Close();
}

float fsrInterface::getMeasurement(int unit) {
    return -1.0f;
}
