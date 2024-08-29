# file stuff is not used rn
file(GLOB_RECURSE CppLinuxSerial_SRC
        "*.cpp")

file(GLOB_RECURSE CppLinuxSerial_HEADERS
        "${CMAKE_SOURCE_DIR}/include/*.hpp")

# Portion of file used begins here <--

add_library(CppLinuxSerial 
	STATIC 
	SerialPort.hpp
	Exception.hpp
	SerialPort.cpp
)

target_include_directories(CppLinuxSerial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})