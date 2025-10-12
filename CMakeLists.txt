cmake_minimum_required(VERSION 3.13)

project(avl)

add_subdirectory(avltree)
add_subdirectory(stdset)
add_subdirectory(tests)
add_subdirectory(benchmark)

option(ENABLE_ASAN OFF)

foreach(target avltree stdset tests benchmark)
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
    target_compile_options(avltree PRIVATE $<$<CONFIG:Debug>: -fsanitize=address>)
    target_link_options(avltree PRIVATE $<$<CONFIG:Debug>: -fsanitize=address>)
endif()