cmake_minimum_required(VERSION 3.3)
project(TIN_iptables)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -W -Wall -Wextra -pedantic")

set(SOURCE_FILES main.cpp HttpServer.cpp HttpServer.h HttpRequest.cpp HttpRequest.h Configuration.cpp Configuration.h IPTablesExecutor.cpp IPTablesExecutor.h Logger/Logger.cpp Logger/Logger.h Logger/Log.cpp Logger/Log.h)
add_executable(TIN_iptables ${SOURCE_FILES})

