cmake_minimum_required(VERSION 3.3)
project(TIN_iptables)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(TIN_iptables ${SOURCE_FILES})