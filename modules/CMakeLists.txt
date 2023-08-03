file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.c include/*.h)
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
add_library(modules STATIC ${srcs})
target_include_directories(modules PUBLIC include)