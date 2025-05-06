include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    club_tests
    club_tests.cpp
)

target_include_directories(club_tests PUBLIC ../include)

target_link_libraries(
    club_tests
    club
    GTest::gtest_main
)                    

include(GoogleTest)

gtest_discover_tests(club_tests)