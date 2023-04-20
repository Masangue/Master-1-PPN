cmake_minimum_required(VERSION 3.16)

project(projet  DESCRIPTION "CNN" LANGUAGES C)
# set(CMAKE_PREFIX_PATH "/home/trgk/spack/spack/opt/spack/linux-arch-zen2/gcc-12.2.1/scorep-8.0-pj3eujh24e7heb4p3ufqyvshk3ua274j/bin/scorep")

# set(CMAKE_C_COMPILER "mpicc")
# set(CMAKE_C_COMPILER "scorep-mpi mpicc")
set(CMAKE_C_STANDARD_REQUIRED ON)




set(CMAKE_BUILD_TYPE Debug)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -g -ggdb -gdwarf  -g3 -Wall -Wextra  -march=native -Ofast -mtune=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -ggdb -gdwarf  -g3 -Wall -Wextra   -march=native -Ofast -mtune=native")
set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf")

add_subdirectory(extern)

add_subdirectory(src)
add_subdirectory(apps)

