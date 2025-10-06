add_executable(avltree)

target_include_directories(avltree PRIVATE ${PROJECT_SOURCE_DIR})

target_sources(avltree PRIVATE
    main.cpp
)