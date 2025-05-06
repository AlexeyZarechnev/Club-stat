cmake_minimum_required(VERSION 4.0.0)
project(
    club-stat
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

enable_testing()

add_subdirectory(lib/club)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC lib/club/include)
target_link_libraries(${PROJECT_NAME} PUBLIC club)