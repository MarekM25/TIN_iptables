#!/bin/bash

function if_cmake_exists()
{
    cmake_exists=false
    if type "cmake" > /dev/null 2>&1; then
        cmake_exists=true
    fi
}

function install_cmake()
{
    apt-get update
    apt-get install -y cmake
}

if_cmake_exists

if [ "$cmake_exists" = false ] ; then
    install_cmake
fi

cmake .
make
