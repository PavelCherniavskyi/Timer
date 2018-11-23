cmake_minimum_required(VERSION 2.8)

project(Timer)
find_package(Boost 1.42.0 REQUIRED COMPONENTS system thread regex filesystem)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -pthread")
add_executable(${PROJECT_NAME} main.cpp Timer.h)
target_link_libraries(${PROJECT_NAME})
