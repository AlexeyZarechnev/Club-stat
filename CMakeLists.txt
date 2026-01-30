cmake_minimum_required(VERSION 3.24)
project(
    club-stat
    VERSION 2.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

if (NEED_SANITIZED)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_subdirectory(lib/club)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC lib/club/include)
target_link_libraries(${PROJECT_NAME} club)

add_executable(main_tests "test/tests.cpp")
target_link_libraries(
    main_tests
    GTest::gtest
    GTest::gtest_main
)  

gtest_discover_tests(main_tests)
