cmake_minimum_required(VERSION 3.3)
project(TIN_iptables)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -W -Wall -Wextra -pedantic")

include_directories(3rdParty/jsoncpp)

set(SOURCE_FILES main.cpp Http/HttpServer.cpp Http/HttpServer.h Http/HttpRequest.cpp Http/HttpRequest.h Configuration/Configuration.cpp Configuration/Configuration.h IPTables/IPTablesExecutor.cpp IPTables/IPTablesExecutor.h Logger/Logger.cpp Logger/Logger.h Logger/Log.cpp Logger/Log.h Extensions/StringExtensions.cpp Extensions/StringExtensions.h Http/HttpResponse.cpp Http/HttpResponse.h Exception/HttpException.cpp Exception/HttpException.h Authorization/auth.cpp Authorization/auth.h Authorization/md5.cpp Authorization/md5.h 3rdParty/jsoncpp/jsoncpp.cpp Http/HttpRequestContext.cpp Http/HttpRequestContext.h Handler/handler.cpp Handler/handler.h Exception/ConfigurationException.cpp Exception/ConfigurationException.h Http/HttpHeader.cpp Http/HttpHeader.h Http/HttpHeaderCollection.cpp Http/HttpHeaderCollection.h CommandLineArgument/CommandLineArgumentCollection.cpp CommandLineArgument/CommandLineArgumentCollection.h CommandLineArgument/CommandLineArgument.cpp CommandLineArgument/CommandLineArgument.h Exception/CommandLineArgumentException.cpp Exception/CommandLineArgumentException.h CommandLineArgument/CommandLineArguments.cpp CommandLineArgument/CommandLineArguments.h)

add_executable(TIN_iptables ${SOURCE_FILES})

file(COPY "iptables.conf" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")