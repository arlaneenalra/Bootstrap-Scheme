
project(bootstrap C)

cmake_minimum_required(VERSION 2.6)

INCLUDE(CPack)

set(VERSION 0.2.0)

add_subdirectory(src)

set(CMAKE_REQUIRED_FLAGS -Wall)


