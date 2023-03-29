cmake_minimum_required(VERSION 3.16)

project(projet  DESCRIPTION "CNN" LANGUAGES C)


# set(CMAKE_C_COMPILER "mpicc")
set(CMAKE_C_STANDARD_REQUIRED ON)


add_subdirectory(extern)


set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -g3 -Wall -Wextra  -march=native -Ofast -mtune=native")


add_subdirectory(src)
add_subdirectory(apps)

