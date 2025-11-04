cmake_minimum_required(VERSION 3.16)
project(Neville2.0)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/opencv-install/lib/cmake/opencv4)

find_package(OpenCV REQUIRED)

add_library(serial STATIC
    dependencies/serial/src/serial.cc
)

if (APPLE)
    target_sources(serial PRIVATE
        dependencies/serial/src/impl/unix.cc
        dependencies/serial/src/impl/list_ports/list_ports_osx.cc
    )
elseif (UNIX)
    target_sources(serial PRIVATE
        dependencies/serial/src/impl/unix.cc
        dependencies/serial/src/impl/list_ports/list_ports_linux.cc
    )
endif()

file(GLOB SRC_FILES src/*)
add_executable(main ${SRC_FILES})

target_link_libraries(main PRIVATE ${OpenCV_LIBS} serial)

target_include_directories(main PRIVATE 
    ${OpenCV_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/include
)
target_include_directories(serial PUBLIC dependencies/serial/include)
