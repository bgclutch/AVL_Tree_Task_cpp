find_package(GTest REQUIRED)

add_executable(tests
    UnitTests/test.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR} ../avltree
)

target_link_libraries(tests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

enable_testing()
add_test(NAME unit_test COMMAND tests)
