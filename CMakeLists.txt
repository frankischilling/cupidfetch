cmake_minimum_required(VERSION 3.27)
project(cupidfetch C)

set(CMAKE_C_STANDARD 11)

add_executable(cupidfetch main.c)

# Assuming ini.c and ini.h are in the same directory as your main.c
target_sources(cupidfetch PRIVATE libs/ini.c)

# Link against the ini library
target_link_libraries(cupidfetch PRIVATE m)  # You may need to link against the math library

