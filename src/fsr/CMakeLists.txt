add_library(fsr
	STATIC
	fsr_driver_serial.h
	fsr_driver_serial.cpp
)

target_link_libraries(fsr PUBLIC CppLinuxSerial)

target_include_directories(fsr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_include_directories(loadCell PUBLIC /usr/local/include)

# target_include_directories(loadCell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../CppLinuxSerial/include)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../CppLinuxSerial/ ${CMAKE_CURRENT_SOURCE_DIR})

