set(TEST_SOURCE_FILES
    option_must_have_at_least_short_or_long_name.cpp
    invalid_format_for_short_option_name.cpp
    invalid_format_for_long_option_name.cpp
    accessing_option_without_arguments.cpp
    option_is_required_but_not_added.cpp
    accessing_option_not_yet_added.cpp
    option_already_added_as.cpp
    option_expects_argument.cpp
    unrecognized_option.cpp)

foreach(TEST_SOURCE_FILE ${TEST_SOURCE_FILES})

    cmake_path(GET TEST_SOURCE_FILE FILENAME TEST_NAME)

    make_test(${TEST_NAME} ${TEST_SOURCE_FILE})

endforeach()
