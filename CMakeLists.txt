cmake_minimum_required(VERSION 3.27)
project(OpenMpiTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenMpiTest mpi_scan_demo.cpp)
