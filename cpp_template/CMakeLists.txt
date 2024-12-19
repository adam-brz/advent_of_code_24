cmake_minimum_required(VERSION 3.20)
project(aoc_xx VERSION 0.1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    message(STATUS "Setting CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
endif()

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD "20" CACHE STRING "Required c++ standard." FORCE)
    message(STATUS "Setting CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")
endif()

enable_testing()

