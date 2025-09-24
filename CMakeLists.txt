cmake_minimum_required(VERSION 3.10.0)
project(wlib VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(wlib main.c)

