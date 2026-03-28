cmake_minimum_required(VERSION 3.10)
project(Arkanoid LANGUAGES C)

#
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#
add_executable(Arkanoid src/main.c)

#
target_include_directories(Arkanoid PRIVATE include)

#
if(WIN32)
	target_link_libraries(Arkanoid user32 gdi32)
endif()
