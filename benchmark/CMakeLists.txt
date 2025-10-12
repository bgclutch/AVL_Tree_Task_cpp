add_executable(benchmark)

target_include_directories(benchmark PRIVATE ${PROJECT_SOURCE_DIR}/avltree)

target_sources(benchmark PRIVATE
    benchmark.cpp
)