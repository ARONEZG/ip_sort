cmake_minimum_required(VERSION 3.22)
project(ip_sort )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ip_sort main.cpp tests.h)

target_compile_options(ip_sort PRIVATE -Wall -Wextra -Wpedantic -Werror)