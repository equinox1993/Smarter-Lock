# To build libpackets

cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11")

project(packets)

file(GLOB_RECURSE PACKETS_CPP
    "*.cpp"
)

add_library(packets SHARED ${PACKETS_CPP})