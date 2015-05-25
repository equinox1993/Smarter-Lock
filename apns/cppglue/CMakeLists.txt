# To build libpackets

cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11")

project(cppglue)

file(GLOB PACKETS_CPP
    "../../Common/Packets/*.cpp"
)

add_library(cppglue SHARED ${PACKETS_CPP} glue.cpp)

add_executable(gluetest glue_test.cpp)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries(cppglue ${OPENSSL_LIBRARIES})
else(OPENSSL_FOUND)
    message("Error: OpenSSL not found")
endif(OPENSSL_FOUND)

target_link_libraries(gluetest cppglue)