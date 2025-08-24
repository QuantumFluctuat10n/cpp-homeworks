add_executable(ProjectManager
    src/main.cpp
    src/cli_handler.cpp
)

target_include_directories(ProjectManager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ProjectManager
    PRIVATE
        core
)