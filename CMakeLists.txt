cmake_minimum_required(VERSION 3.28)
project(IP_CATALOG C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/Cellar/gtk4/4.12.4/lib/pkgconfig")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

add_executable(IP_CATALOG main.c include/conf.inc.h)
target_link_libraries(IP_CATALOG PkgConfig::GTK4)