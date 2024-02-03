cmake_minimum_required(VERSION 2.8.0)

file(GLOB_RECURSE c_srcs CONFIGURE_DEPENDS src/*.c include/*.h)
file(GLOB_RECURSE cxx_srcs CONFIGURE_DEPENDS src/*.c* include/*.h*)

include_directories(include)

add_library(AZH_Network_Program STATIC ${c_srcs})
add_library(AZH_Network_Program_CXX STATIC ${cxx_srcs})

if(CMAKE_HOST_WIN32)
    target_link_libraries(AZH_Network_Program ws2_32.lib)
    target_link_libraries(AZH_Network_Program_CXX ws2_32.lib)
endif()

add_subdirectory(test)