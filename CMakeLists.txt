cmake_minimum_required(VERSION 3.3)

project(ProjectM5)

set(HEADER_FILES headers/ftd2xx.h)
link_libraries(ftd2xx)
add_executable(ProjectM5 src/main.c)