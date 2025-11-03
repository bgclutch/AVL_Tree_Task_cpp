cmake_minimum_required(VERSION 3.13)

project(avl)

add_subdirectory(avltree)
add_subdirectory(stdset)
add_subdirectory(tests)

option(ENABLE_ASAN OFF)
option(ENABLE_BENCHMARK OFF)

if(ENABLE_BENCHMARK)
    add_subdirectory(benchmark)
endif()

foreach(target avltree stdset tests)
    target_compile_features(${target} PUBLIC cxx_std_23)
endforeach()

target_compile_options(avltree PRIVATE
    -O2
$<$<CONFIG:Debug>:
    -Wall
    -Wextra
    -Wpedantic
    -DDEBUG
    -Wmissing-declarations
    -Wcast-align
    -Wshadow
    -Wsign-conversion>
)

if(ENABLE_ASAN)
    foreach(target avltree tests)
        target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>: -fsanitize=address>)
        target_link_options(${target} PRIVATE $<$<CONFIG:Debug>: -fsanitize=address>)
    endforeach()
endif()