cmake_minimum_required(VERSION 3.27)
project(MpiMatrixMultiply)

set(CMAKE_C_STANDARD 17)

find_package(MPI REQUIRED)

add_executable(MpiMatrixMultiply main.c
        generator.h)

target_link_libraries(MpiMatrixMultiply MPI::MPI_CXX)
