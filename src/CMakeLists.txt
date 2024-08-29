add_subdirectory(fsr)
add_subdirectory(CppLinuxSerial)

# Load Cell ADC Library for ADC ADS1262 (maybe ADS1261, forgot. think 2)
add_library(fsrinterface
STATIC
driverInterface.h
fsrInterface.h
fsrInterface.cpp)

target_link_libraries(fsrinterface PUBLIC fsr)

