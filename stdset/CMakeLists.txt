add_executable(stdset)

target_include_directories(stdset PRIVATE ${PROJECT_SOURCE_DIR})

target_sources(stdset PRIVATE
    main.cpp
)