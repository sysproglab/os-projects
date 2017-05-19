cmake_minimum_required(VERSION 3.7)
project(inot)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES inot.c)
set(SOURCE_STATIC static.c)
set(SOURCE_DYNAMIC dynamic.c)

add_library(static STATIC ${SOURCE_STATIC})
add_library(dynamic SHARED ${SOURCE_DYNAMIC})

add_executable(inot ${SOURCE_FILES})

add_dependencies(inot static dynamic)

target_link_libraries(static dynamic)
target_link_libraries(inot static)
target_link_libraries(inot dynamic)
