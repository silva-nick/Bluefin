cmake_minimum_required(VERSION 3.7.0...3.22.0)

if(${CMAKE_VERSION} VERSION_LESS 3.12.0)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Bluefin VERSION 0.1.0 LANGUAGES CXX)
set(VERSION_SUFFIX "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "")

set(ENABLE_TEST_SUITE ON CACHE BOOL "Enable the test suite")

# For IDE
file(GLOB_RECURSE ALL_HEADER_FILES "*.hpp")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)

# gtest
if(ENABLE_TEST_SUITE)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
