cmake_minimum_required(VERSION 3.0)
project(P1 C)

# Specify the C standard (change to your desired standard)
set(CMAKE_C_STANDARD 99)

# Add your header files
set(HEADERS
    dynamic_list.h
    types.h
)

# Add your source files
set(SOURCES
    p0.c
    dynamic_list.c
)

# Create an executable target
add_executable(P1-Executable ${SOURCES} ${HEADERS})

