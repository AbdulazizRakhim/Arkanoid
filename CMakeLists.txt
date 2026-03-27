# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

project ("New Arkanoid" LANGUAGES C)

# Include sub-projects.
add_subdirectory ("New Arkanoid")
