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

include(CTest)

if(BUILD_TESTING)
  include(FetchContent)
  FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.15.2
)
  FetchContent_MakeAvailable(Catch2)

  add_executable(PADT_tests
        test/weights_test.cpp
    )

  target_link_libraries(PADT_tests
        PRIVATE
            PADT::PADT
            Catch2::Catch2WithMain
    )

  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  include(Catch)
  catch_discover_tests(PADT_tests)
endif()


add_executable(PADT_example examples/main.cpp)
target_link_libraries(PADT_example PRIVATE PADT::PADT)
