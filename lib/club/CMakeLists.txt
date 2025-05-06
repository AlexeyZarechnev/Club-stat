add_library(club src/club.cpp)
target_include_directories(club PUBLIC include)

add_subdirectory(test)