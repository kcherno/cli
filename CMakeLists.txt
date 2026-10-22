cmake_minimum_required(VERSION 3.23)

project(cli LANGUAGES CXX VERSION 1.0.2)

option(enable_cli_unit_tests "enable unit tests" OFF)

add_library(${PROJECT_NAME} SHARED)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED    ON
    LIBRARY_OUTPUT_DIRECTORY lib
    SOVERSION                ${PROJECT_VERSION_MAJOR}
    VERSION                  ${PROJECT_VERSION}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_compile_options(${PROJECT_NAME}
    PRIVATE "$<$<CXX_COMPILER_ID:GNU>:--debug;-Wall;-Werror;-Wextra>")

target_sources(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/detail/src/verification.cpp

    PRIVATE ${PROJECT_SOURCE_DIR}/src/parsed_options.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/options.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/option.cpp
    PRIVATE ${PROJECT_SOURCE_DIR}/src/parser.cpp

    PUBLIC FILE_SET HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR}
)

if (${enable_cli_unit_tests})

    enable_testing()

    add_subdirectory(test)

endif()
