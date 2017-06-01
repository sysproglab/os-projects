cmake_minimum_required(VERSION 3.5)
project(task_manager)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp ProcessInfo.cpp ProcessInfo.h ProcessTable.cpp ProcessTable.h Logger.cpp Logger.h Util.h)


find_package (Threads)
add_executable(proc-logger ${SOURCE_FILES})
target_link_libraries (proc-logger ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS proc-logger EXPORT proc-logger DESTINATION bin)