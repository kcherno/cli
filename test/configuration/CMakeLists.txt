set(TEST_SOURCE_FILES exception_source_information.cpp)

foreach(TEST_SOURCE_FILE ${TEST_SOURCE_FILES})

    cmake_path(GET TEST_SOURCE_FILE FILENAME TEST_NAME)

    make_test(${TEST_NAME} ${TEST_SOURCE_FILE})

endforeach()
