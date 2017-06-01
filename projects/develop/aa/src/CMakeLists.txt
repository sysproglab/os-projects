cmake_minimum_required(VERSION 3.7)
project(xxonwm)
set(CMAKE_C_STANDARD 99)

# includes for wlc
include_directories(/usr/include)
include_directories(/usr/local/include)
include_directories(/usr/local/include/lib/chck)
link_directories(/usr/local/lib64/)

# include inih
add_subdirectory(inih)
include_directories(inih)
link_directories(inih)


set(SOURCE_FILES main.c config.c)
add_executable(xxonwm ${SOURCE_FILES})

target_link_libraries(xxonwm wlc inih)
