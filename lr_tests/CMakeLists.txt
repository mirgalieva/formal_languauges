include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)
add_executable(lr_tests lr_test.cpp)

target_link_libraries(lr_tests
        PRIVATE
        GTest::GTest
        lr)

add_test(multiply_gtests lr_tests)