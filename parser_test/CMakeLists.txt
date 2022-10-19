include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
add_executable(parser_test parser_test.cpp)

target_link_libraries(parser_test
        PRIVATE
        GTest::GTest
        parser)

add_test(multiply_gtests parser_test)