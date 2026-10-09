find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(boost_cli_unit_test_options --log_level=all)

function(add_cli_boost_unit_test source_file)

    get_filename_component(filename ${source_file} NAME_WLE)

    add_executable(${filename} ${source_file})

    target_link_libraries(${filename}
        PRIVATE Boost::unit_test_framework
        PRIVATE cli
    )

    add_test(NAME ${filename} COMMAND ${filename} ${boost_cli_unit_test_options})

endfunction()

add_cli_boost_unit_test(options.cpp)
add_cli_boost_unit_test(option.cpp)
add_cli_boost_unit_test(parser.cpp)

add_subdirectory(detail)
