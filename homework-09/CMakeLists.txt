cmake_minimum_required(VERSION 3.20)

project(ProjectManager)

set(CONAN_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/build/Release/generators/conan_toolchain.cmake)
set(CMAKE_BUILD_TYPE Release)

include(${CONAN_TOOLCHAIN_FILE})

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)
find_package(GTest REQUIRED)
find_package(fmt REQUIRED)

add_subdirectory(src/core)
add_subdirectory(src/app)

target_link_libraries(ProjectManager
    PUBLIC
    nlohmann_json::nlohmann_json
    PRIVATE
    spdlog::spdlog
    CLI11::CLI11
    gtest::gtest
    fmt::fmt)