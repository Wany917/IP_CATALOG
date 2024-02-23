cmake_minimum_required(VERSION 3.28)
project(IP_CATALOG C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# Définir le chemin pour pkg-config
set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/Cellar/gtk4/4.12.4/lib/pkgconfig:/opt/homebrew/Cellar/sqlite/3.45.1/lib/pkgconfig")

# Trouver GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

# Trouver SQLite
pkg_check_modules(SQLITE3 REQUIRED sqlite3)

# Ajouter les répertoires d'inclusion de GTK4 et SQLite
include_directories(${GTK4_INCLUDE_DIRS} ${SQLITE3_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS} ${SQLITE3_LIBRARY_DIRS})

# Ajouter les répertoires d'inclusion
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/define)

# Définir l'exécutable avec tous les fichiers source nécessaires
add_executable(IP_CATALOG main.c function/function.c database/initdb.c)

# Lier les bibliothèques GTK4 et SQLite
target_link_libraries(IP_CATALOG PkgConfig::GTK4 ${SQLITE3_LIBRARIES})
