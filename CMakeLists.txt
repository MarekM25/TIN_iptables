cmake_minimum_required(VERSION 3.3)
project(TIN_iptables)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp Http/HttpServer.cpp Http/HttpServer.h Http/HttpRequest.cpp Http/HttpRequest.h Configuration.cpp Configuration.h IPTablesExecutor.cpp IPTablesExecutor.h Logger.cpp Logger.h Extensions/StringExtensions.cpp Extensions/StringExtensions.h Http/HttpResponse.cpp Http/HttpResponse.h)
add_executable(TIN_iptables ${SOURCE_FILES})