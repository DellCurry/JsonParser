cmake_minimum_required(VERSION 3.10.2)
project(JsonParser)
set(CMAKE_CXX_STANDARD 11)
FILE(GLOB SC_FILES "*.cpp" "*.h")
set(CMAKE_CXX_FLAGS "-O3 -g -Wall -Wextra")
add_executable(json ${SC_FILES})


