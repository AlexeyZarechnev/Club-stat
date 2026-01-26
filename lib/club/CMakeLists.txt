
file(GLOB LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(club ${LIB_SOURCES})
target_include_directories(club PUBLIC include)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS test/*.cpp)
add_executable(
    tests
    ${TEST_SOURCES}
)

target_include_directories(tests PUBLIC include)

target_link_libraries(
    tests
    club
)

target_link_libraries(
    tests
    GTest::gtest
    GTest::gtest_main
)                    

include(GoogleTest)

gtest_discover_tests(tests)