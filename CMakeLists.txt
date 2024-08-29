cmake_minimum_required(VERSION 3.0.0)

project(ForceFieldFSR VERSION 1.0 LANGUAGES CXX C)

add_executable(ForceFieldFSR src/main.cpp)

add_subdirectory(src)

target_link_libraries(ForceFieldFSR PUBLIC fsrinterface)

set(CMAKE_SYSTEM_NAME Linux)

set_target_properties(ForceFieldFSR PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "bin"
    ARCHIVE_OUTPUT_DIRECTORY "lib"
    LIBRARY_OUTPUT_DIRECTORY "lib"
)
