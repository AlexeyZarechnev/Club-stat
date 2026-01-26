cmake_minimum_required(VERSION 4.0.0)
project(
    club-stat
    VERSION 2.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(lib/club)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC lib/club/include)
target_link_libraries(${PROJECT_NAME} club)