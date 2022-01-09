cmake_minimum_required(VERSION 3.15)

project(StringSliceTests)

add_executable(StringSliceTests StringSlice_test.cpp)
target_include_directories(StringSliceTests PRIVATE ..)
