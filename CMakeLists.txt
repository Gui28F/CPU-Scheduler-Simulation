# cmake_minimum_required(VERSION <specify CMake version here>)
project(CPUs C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(CPUs
        communications.c
        communications.h
        CPU.c
        globals.h
        JobSch.c
        scheduler.c
        scheduler.h)
