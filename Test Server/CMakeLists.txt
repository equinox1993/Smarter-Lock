cmake_minimum_required(VERSION 2.8)

project(Server)
add_definitions("-std=c++11")

add_executable(Server main.cpp)

find_package(OpenCV)
target_link_libraries(Server ${OpenCV_LIBRARIES})

find_package(OpenSSL)
target_link_libraries(Server ${OpenSSL_LIBRARIES})