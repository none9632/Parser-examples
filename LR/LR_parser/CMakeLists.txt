cmake_minimum_required(VERSION 3.15)
project(LR C)

file(GLOB SOURCES "src/*.c")
file(GLOB HEADERS "include/*.h")

add_executable(LR ${SOURCES} ${HEADERS})
