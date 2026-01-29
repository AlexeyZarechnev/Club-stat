
file(GLOB LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(club ${LIB_SOURCES})
target_include_directories(club PUBLIC include)

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

gtest_discover_tests(tests)