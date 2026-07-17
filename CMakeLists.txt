cmake_minimum_required(VERSION 3.24...4.0)

project(
    PADT
    VERSION 0.1.0
    DESCRIPTION "Preliminary Aircraft Design Tool"
    LANGUAGES CXX
)

add_library(PADT
    src/weights.cpp
)

add_library(PADT::PADT ALIAS PADT)

target_compile_features(PADT PUBLIC cxx_std_23)

set_target_properties(PADT PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(PADT
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(PADT PRIVATE /W4)
else()
    target_compile_options(PADT PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(PADT_example examples/main.cpp)
target_link_libraries(PADT_example PRIVATE PADT::PADT)
