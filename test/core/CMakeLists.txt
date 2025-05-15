set(TEST_SOURCE_FILES
    dictionary.cpp
    option_map.cpp
    option.cpp
    parser.cpp)

foreach(TEST_SOURCE_FILE ${TEST_SOURCE_FILES})

    cmake_path(GET TEST_SOURCE_FILE FILENAME TEST_NAME)

    make_test(${TEST_NAME} ${TEST_SOURCE_FILE})

endforeach()
