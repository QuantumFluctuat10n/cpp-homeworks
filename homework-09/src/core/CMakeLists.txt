add_library(core STATIC
    src/project.cpp
    src/task.cpp
    src/storage.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)


target_link_libraries(core 
        PUBLIC 
        nlohmann_json::nlohmann_json
        PRIVATE
        fmt::fmt
        spdlog::spdlog)