cmake_minimum_required(VERSION 3.16)

project(projet  DESCRIPTION "CNN" LANGUAGES C)

set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)


# Fetch lbspng
include(FetchContent)
FetchContent_Declare(
  libspng
  GIT_REPOSITORY https://github.com/randy408/libspng.git
  GIT_TAG        75c39ce0948d3b9c623a674ebfeb63a703a7c116
)
FetchContent_MakeAvailable(libspng)




set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -march=native -Ofast -mtune=native")
# set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")

add_subdirectory(src)

 add_subdirectory(apps)

