cmake_minimum_required(VERSION 3.27.0)

project(cli)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD          20)

include_directories(AFTER ${CMAKE_SOURCE_DIR}/include)

set(BUILD_TESTING ON)

if (BUILD_TESTING)

    enable_testing()

    add_subdirectory(${CMAKE_SOURCE_DIR}/test)

endif()
