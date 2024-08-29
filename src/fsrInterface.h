/**
 * @brief       Header for FSR interface
 * 
 * Written to work with USB now
 * 
 * @author      John
 * 
 */

#ifndef FSRINTERFACE_H
#define FSRINTERFACE_H

#include "driverInterface.h"
#include "fsr_driver_serial.h"
#include "CppLinuxSerial/SerialPort.hpp"

class fsrInterface : public DriverInterface {
public:
    fsrInterface();

    /**
     * @brief Initialize the FSR driver and USB connection.
     */
    void initialize();

    /**
     * @brief Update the state of the FSR sensor by reading from the USB port.
     */
    void updateSensorState();

    /**
     * @brief Get the reading from a particular channel of the load cell.
     * 
     * @param channelNum The channel number (0-5).
     * @return The reading from the specified channel.
     */
    float getChannelReading(int channelNum);

    /**
     * @brief Shutdown the FSR interface (if necessary).
     */
    void shutdown();

    /**
     * @brief Get a measurement in a specified unit (not implemented in this case).
     * 
     * @param unit The unit for measurement.
     * @return -1 as this is not implemented.
     */
    float getMeasurement(int unit);

private:
    mn::CppLinuxSerial::SerialPort serialPort;      ///< Serial port object for USB communication
    fsr_driver_serial fsrDriver;                    ///< FSR driver object for handling the serial communication
    std::vector<uint16_t> sensorData;               ///< Buffer to hold the latest sensor data
};

#endif // FSRINTERFACE_H
