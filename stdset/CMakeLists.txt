add_executable(stdset)

target_include_directories(stdset PRIVATE ${PROJECT_SOURCE_DIR})

target_compile_options(stdset PRIVATE
    -O2
)

target_sources(stdset PRIVATE
    main.cpp
)