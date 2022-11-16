cmake_minimum_required(VERSION 3.23)
project(cityDispatcher C)

set(CMAKE_C_STANDARD 99)

add_executable(cityDispatcher src/main.c)
add_executable(utilityResponder src/CityUtility.c)

add_subdirectory (include)
add_subdirectory (src)