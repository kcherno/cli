set(TEST_SOURCE_FILES
    option_book.cpp
    option.cpp
    parser.cpp)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/core/parser.cpp)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

foreach(TEST_SOURCE_FILE ${TEST_SOURCE_FILES})

    cmake_path(GET TEST_SOURCE_FILE FILENAME TEST_NAME)

    add_executable(${TEST_NAME} ${TEST_SOURCE_FILE} ${SOURCE_FILES})

    if (${TEST_SOURCE_FILE} STREQUAL option.cpp)

	target_compile_options(${TEST_NAME} PRIVATE -D NDEBUG)

    endif()

    target_link_libraries(${TEST_NAME} PRIVATE Boost::unit_test_framework)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} --log_level=all)

endforeach()
