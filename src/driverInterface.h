/**
 * @file	driverInterface.h
 * @brief	File for parent class of other driver interfaces
 */

#ifndef DRIVERINTERFACE_H
#define DRIVERINTERFACE_H

/**
 * @brief 	Parent class of driver interfaces (joint encoder, load cell, etc)
 */
class DriverInterface{
    public:
		/**
		 * @brief 	Initialize sensor object
		 */
        virtual void initialize()=0; //eventually will need to also handle port or "slave" as it is actually. e.g., Bus 1 slave 0

		/**
		 * @brief 	Shutdown sensor object
		 */
        virtual void shutdown()=0;

		/**
		 * @brief	Take measurement from sensor
		 */
        virtual float getMeasurement(int unit)=0;

		/**
		 * @brief	Update internal values that sensor may need (ex. moving avg)
		 */
        virtual void updateSensorState()=0;
};
#endif //DRIVERINTERFACE_H
