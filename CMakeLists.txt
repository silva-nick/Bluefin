cmake_minimum_required(VERSION 3.7.0...3.22.0)

if(${CMAKE_VERSION} VERSION_LESS 3.12.0)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Bluefin VERSION 0.1.0 LANGUAGES CXX)
set(VERSION_SUFFIX "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set default compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wunreachable-code -pedantic -g")

# generate coverage
option(CODE_COVERAGE "Enable Coverage report" OFF)

if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    #set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wunreachable-code -g -O0 --coverage")
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wunreachable-code -g -O0 --coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
endif()

# boost
set(Boost_NO_SYSTEM_PATHS true)
set(Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static libraries from Boost")
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF) 
set(Boost_DEBUG ON)
find_package(Boost REQUIRED)

if(Boost_FOUND)
  message(WARNING "BOOST FOUND")
  include_directories(${Boost_INCLUDE_DIRS})
  link_libraries(${Boost_LIBRARIES})
  set(BOOST_LIBS ${Boost_LIBRARIES})
else()
  set(BOOST_LIBS )
  message(WARNING "BOOST NOT FOUND")
endif()

add_subdirectory(src)

# gtest
option(ENABLE_TEST_SUITE "Enable the test suite" ON)

if(ENABLE_TEST_SUITE AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    enable_testing()
    include(GoogleTest)
    include(CTest)
    add_subdirectory(tests)
endif()

# For IDE
file(GLOB_RECURSE ALL_HEADER_FILES "*.hpp")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
