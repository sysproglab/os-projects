cmake_minimum_required(VERSION 3.7)
project(daemon)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig)
find_package (Qt5 CONFIG REQUIRED DBus)
find_package(PkgConfig)

pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(/usr/include/dbus-1.0/)
include_directories(/usr/lib/x86_64-linux-gnu/dbus-1.0/include)

set (LIBS dbus-1 dbus-glib-1)
set(SOURCE_FILES main.cpp Struct.h)

add_executable(calls_daemon ${SOURCE_FILES})

target_link_libraries(calls_daemon Qt5::DBus ${DBUS_LIBRARIES} ${GLIB_LIBRARIES} ${LIBS})
