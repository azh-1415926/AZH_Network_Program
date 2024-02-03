cmake_minimum_required(VERSION 2.8.0)
project(Http)

message(STATUS "The platform is ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "The project name is ${PROJECT_NAME}")

add_executable(Http client.c)
add_executable(Http_CXX client.cpp)

target_link_libraries(Http PRIVATE AZH_Network_Program)
target_link_libraries(Http_CXX PRIVATE AZH_Network_Program_CXX)