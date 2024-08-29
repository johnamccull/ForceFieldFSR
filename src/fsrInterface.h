#ifndef FSRINTERFACE_H
#define FSRINTERFACE_H

#include "driverInterface.h"
#include "fsr_driver_serial.h"
#include "CppLinuxSerial/SerialPort.hpp"

class fsrInterface : public DriverInterface {
public:
    fsrInterface();

    void initialize();
    void updateSensorState();
    float getChannelReading(int channelNum);
    void shutdown();
    float getMeasurement(int unit);

private:
    mn::CppLinuxSerial::SerialPort serialPort;
    fsr_driver_serial fsrDriver;
    std::vector<uint16_t> sensorData{3};
};

#endif // FSRINTERFACE_H
