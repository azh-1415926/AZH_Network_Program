cmake_minimum_required(VERSION 2.8.0)
project(Http)

message(STATUS "The platform is ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "The project name is ${PROJECT_NAME}")
message(STATUS "The source is ${SOURCEFILES}")

add_executable(${PROJECT_NAME} client.c)

target_link_libraries(${PROJECT_NAME} PRIVATE AZH_Network_Program)