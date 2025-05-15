find_package(Boost REQUIRED COMPONENTS unit_test_framework)

function(make_test TEST_NAME TEST_SOURCE_FILE)

    add_executable(${TEST_NAME} ${TEST_SOURCE_FILE})

    target_include_directories(${TEST_NAME} PRIVATE ${INCLUDE_DIRECTORIES})

    target_link_libraries(${TEST_NAME}
	PRIVATE Boost::unit_test_framework
	PRIVATE ${PROJECT_NAME})

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} --log_level=all)

endfunction()

add_subdirectory(configuration)
add_subdirectory(core)
add_subdirectory(error)
add_subdirectory(generic)
