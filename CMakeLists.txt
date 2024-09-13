cmake_minimum_required(VERSION 3.26)
project(tester)

set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-Wall")
include_directories(.)

add_executable(c-tester
        tester.h
        tester_assert.h
        example/main.c
        example/test_one.h
        example/test_xyz.h)

add_executable(cpp-tester
        tester.h
        tester_assert.h
        example/main.cpp
        example/test_one.h
        example/test_xyz.h)
