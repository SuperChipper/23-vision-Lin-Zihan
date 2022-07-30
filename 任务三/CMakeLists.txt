cmake_minimum_required(VERSION 3.10)

project(ros_test)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(ros_test main.cpp Detect.cpp)

target_include_directories(ros_test PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(ros_test ${OpenCV_LIBS})
