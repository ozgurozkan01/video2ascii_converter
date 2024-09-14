cmake_minimum_required(VERSION 3.28)
project(video2ascii_converter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(video2ascii_converter main.cpp)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_videoio)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})