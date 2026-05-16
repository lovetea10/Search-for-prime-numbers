cmake_minimum_required(VERSION 3.16)
project(PrimeMPI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск MPI
find_package(MPI REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${MPI_CXX_INCLUDE_DIRS}
)

set(SOURCES
    src/main.cpp
    src/sieve.cpp
)

add_executable(prime_mpi ${SOURCES})

target_compile_options(prime_mpi PRIVATE -O2 -Wall)

target_link_libraries(prime_mpi MPI::MPI_CXX)
