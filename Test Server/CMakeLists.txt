cmake_minimum_required(VERSION 2.8)

project(Server)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeModules")

option(NO_GUI "No gui for server." OFF)

if(NO_GUI)
    add_definitions(-DNO_GUI)
endif(NO_GUI)
add_definitions("-std=c++11")

file(GLOB Packets_CPP
    "../Common/Packets/*.cpp"
)

file(GLOB COMMON_CPP
    "../Common/*.cpp"
)

add_executable(Server main.cpp ${Packets_CPP} ${COMMON_CPP})

find_package(OpenCV 2 REQUIRED)
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIR})
    target_link_libraries(Server ${OpenCV_LIBS})
else(OpenCV_FOUND)
    message("OpenCV not found")
endif(OpenCV_FOUND)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries(Server ${OPENSSL_LIBRARIES})
else(OPENSSL_FOUND)
    message("OpenSSL not found")
endif(OPENSSL_FOUND)

find_package(Threads)
target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})