cmake_minimum_required(VERSION 3.20)
project(COMP_345_PROJECT_TEAM_N12)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(src)

add_executable(COMP_345_PROJECT_TEAM_N12
        src/map/map_driver.cpp)
