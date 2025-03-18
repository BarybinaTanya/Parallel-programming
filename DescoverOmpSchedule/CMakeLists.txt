cmake_minimum_required(VERSION 3.27)
project(PragmaForV C)

set(CMAKE_C_STANDARD 11)

find_package(OpenMP REQUIRED)

add_executable(PragmaForV
        main.c
        generator.h)

if (OpenMP_C_FOUND)
    target_link_libraries(PragmaForV PRIVATE OpenMP::OpenMP_C)
endif()

target_link_libraries(PragmaForV PRIVATE m)
