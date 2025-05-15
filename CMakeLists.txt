cmake_minimum_required(VERSION 3.14.0)

project(cli)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD          20)

set(INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/option_map.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/option.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/parser.cpp)

option(BUILD_UNIT_TESTS "build unit tests for the library" OFF)

option(DISABLE_EXCEPTION_SOURCE_INFORMATION
    "disable exception location information" ON)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY lib)

if (DISABLE_EXCEPTION_SOURCE_INFORMATION)

    target_compile_definitions(${PROJECT_NAME}
	PRIVATE DISABLE_EXCEPTION_SOURCE_INFORMATION)

endif()

target_compile_options(${PROJECT_NAME}
    PRIVATE "$<$<COMPILE_LANG_AND_ID:CXX,GNU>:-g;-O3;-Wall;-Werror;-Wextra;-Wpedantic>")

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES})

if (BUILD_UNIT_TESTS)

    enable_testing()

    add_subdirectory(test)

endif()
