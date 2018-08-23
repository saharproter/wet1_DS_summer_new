cmake_minimum_required(VERSION 3.8)
project(wet1_summer)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main1.cpp library1.h library1.cpp avlTree.h)
add_executable(wet1_summer ${SOURCE_FILES})