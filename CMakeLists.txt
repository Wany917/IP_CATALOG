cmake_minimum_required(VERSION 3.28)
project(IP_CATALOG C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# Inclure les répertoires d'en-tête SDL2
include_directories(/opt/homebrew/include/SDL2)

# Ajouter les chemins de la bibliothèque
link_directories(/opt/homebrew/lib)

add_executable(IP_CATALOG main.c)

# Lier avec SDL2, SDL2_ttf et d'autres bibliothèques nécessaires
target_link_libraries(IP_CATALOG SDL2 SDL2_ttf m dl)
