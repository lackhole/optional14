cmake_minimum_required(VERSION 3.19)
project(lackhole)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")

add_subdirectory(optional14)

add_executable(lackhole main.cc)

target_link_libraries(lackhole optional14)
