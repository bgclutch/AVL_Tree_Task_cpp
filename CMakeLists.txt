cmake_minimum_required(VERSION 3.30)

project(avl)

add_subdirectory(avltree)
add_subdirectory(stdset)
add_subdirectory(tests)

option(ENABLE_ASAN OFF)

foreach(target avltree stdset tests)
    target_compile_features(${target} PUBLIC cxx_std_23)
endforeach()

target_compile_options(avltree PRIVATE
$<$<CONFIG:Debug>:
    -Wall
    -Wextra
    -Wpedantic
    -DDEBUG
    -O2
    -Wmissing-declarations
    -Wcast-align
    -Wshadow
    -Wsign-conversion>
)

if(ENABLE_ASAN)
    target_compile_options(avltree PRIVATE $<$<CONFIG:Debug>: -fsanitize=address>)
    target_link_options(avltree PRIVATE $<$<CONFIG:Debug>: -fsanitize=address>)
endif()