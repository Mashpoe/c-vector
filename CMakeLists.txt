cmake_minimum_required(VERSION 3.23)
project(c-vector C)

set(CMAKE_C_STANDARD 17)

add_library(${PROJECT_NAME} STATIC vec.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/c-vector)
