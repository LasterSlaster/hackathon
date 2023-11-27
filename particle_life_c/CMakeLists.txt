# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(particle_life_c)

# Find SDL2
find_package(SDL2 REQUIRED)

# Add your source files
add_executable(particle_life_c main.c)

# Link SDL2
target_link_libraries(particle_life_c PRIVATE SDL2)
target_link_libraries(particle_life_c PRIVATE m rt)


# Optional: Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
