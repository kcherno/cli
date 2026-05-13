#define BOOST_TEST_MODULE options

#define BOOST_TEST_DYN_LINK

#include <string_view>
#include <stdexcept>
#include <sstream>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "options.hpp"
#include "option.hpp"

BOOST_AUTO_TEST_SUITE(options);

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(default_constructor)
{
    const cli::options options;

    BOOST_TEST(options.description().empty());

    BOOST_TEST(options.empty());

    BOOST_CHECK_EQUAL(options.size(), 0);

    BOOST_TEST((options.begin() == options.end()));
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
    const cli::options options {
        cli::option {.short_name = "-h", .long_name  = "--help"}
    };

    BOOST_TEST(options.description().empty());

    BOOST_TEST(not options.empty());

    BOOST_CHECK_EQUAL(options.size(), 1);

    BOOST_TEST((options.begin() != options.end()));
}

BOOST_AUTO_TEST_SUITE(parameterized_constructor);

BOOST_AUTO_TEST_CASE(initialize_with_string)
{
    const cli::options options {"<project-name> options:"};

    BOOST_TEST(not options.description().empty());

    BOOST_TEST(options.empty());
}

BOOST_AUTO_TEST_CASE(initialize_with_option_does_not_contain_names)
{
    BOOST_CHECK_EXCEPTION(
        cli::options {cli::option {}},
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option must have at least a short or long name";
        }
    );
}

BOOST_AUTO_TEST_CASE(initialize_with_option_containing_invalid_short_name)
{
    BOOST_CHECK_EXCEPTION(
        cli::options {cli::option {.short_name = "h"}},
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "short option name must match the pattern ^-[a-zA-Z0-9]$";
        }
    );
}

BOOST_AUTO_TEST_CASE(initialize_with_option_containing_valid_short_name)
{
    const cli::options options {cli::option {.short_name = "-h"}};

    BOOST_TEST(options.contains("-h"));
}

BOOST_AUTO_TEST_CASE(initialize_with_option_containing_invalid_long_name)
{
    BOOST_CHECK_EXCEPTION(
        cli::options {cli::option {.long_name = "help"}},
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "long option name must match the pattern "
                "^--[a-zA-Z0-9][-a-zA-Z0-9]*[a-zA-Z0-9]$";
        }
    );
}

BOOST_AUTO_TEST_CASE(initialize_with_option_containing_valid_long_name)
{
    const cli::options options {cli::option {.long_name = "--help"}};

    BOOST_TEST(options.contains("--help"));
}

BOOST_AUTO_TEST_CASE(initialize_with_option_containing_invalid_names)
{
    BOOST_CHECK_EXCEPTION(
        (cli::options {cli::option {.short_name = "h", .long_name = "help"}}),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "short option name must match the pattern ^-[a-zA-Z0-9]$";
        }
    );
}

BOOST_AUTO_TEST_CASE(initialize_with_option_containing_valid_names)
{
    const cli::options options {
        cli::option {.short_name = "-h", .long_name = "--help"}
    };

    BOOST_TEST(options.contains("-h"));

    BOOST_TEST(options.contains("--help"));

    BOOST_CHECK_EQUAL(options.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END(); // options/constructor/parameterized_constructor

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    const cli::options options_1 {
        cli::option {.short_name = "-h", .long_name  = "--help"}
    };

    const auto options_2 {options_1};

    BOOST_TEST(not options_1.empty());

    BOOST_TEST(not options_2.empty());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    cli::options options_1 {
        cli::option {.short_name = "-h", .long_name  = "--help"}
    };

    const auto options_2 {std::move(options_1)};

    BOOST_TEST(options_1.empty());

    BOOST_TEST(not options_2.empty());
}

BOOST_AUTO_TEST_SUITE_END(); // options/constructor

BOOST_AUTO_TEST_SUITE(add_option);

BOOST_AUTO_TEST_CASE(that_has_already_been_added)
{
    cli::options options_1 {cli::option {.short_name = "-h"}};

    BOOST_CHECK_EXCEPTION(
        options_1.add_option(cli::option {"-h"}),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option [-h] has already been added as [-h]";
        }
    );

    BOOST_CHECK_EXCEPTION(
        (options_1.add_option(
            cli::option {.short_name = "-h", .long_name = "--help"})),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option [-h, --help] has already been added as [-h]";
        }
    );

    cli::options options_2 {cli::option {.long_name = "--help"}};

    BOOST_CHECK_EXCEPTION(
        options_2.add_option(cli::option {.long_name = "--help"}),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option [--help] has already been added as [--help]";
        }
    );

    BOOST_CHECK_EXCEPTION(
        (options_2.add_option(
            cli::option {.short_name = "-h", .long_name = "--help"})),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option [-h, --help] has already been added as [--help]";
        }
    );

    cli::options options_3 {
        cli::option {.short_name = "-h", .long_name = "--help"}
    };

    BOOST_CHECK_EXCEPTION(
        options_3.add_option(cli::option {.short_name = "-h"}),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option [-h] has already been added as [-h, --help]";
        }
    );

    BOOST_CHECK_EXCEPTION(
        (options_3.add_option(cli::option {.long_name = "--help"})),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option [--help] has already been added as [-h, --help]";
        }
    );

    BOOST_CHECK_EXCEPTION(
        (options_3.add_option(
            cli::option {.short_name = "-h", .long_name = "--help"})),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option [-h, --help] has already been added as [-h, --help]";
        }
    );
}

BOOST_AUTO_TEST_SUITE_END(); // options/add_option

BOOST_AUTO_TEST_SUITE(subscript_operator);

BOOST_AUTO_TEST_CASE(access_to_unadded_option)
{
    const cli::options options {
        cli::option {.short_name = "-h", .long_name = "--help"}
    };

    BOOST_REQUIRE_NO_THROW(options["-h"]);

    BOOST_REQUIRE_NO_THROW(options["--help"]);

    BOOST_CHECK_EXCEPTION(
        options["-f"], std::runtime_error, [](auto&& exception)
        {
            return std::string_view(exception.what()) == "-f was not added";
        }
    );
}

BOOST_AUTO_TEST_SUITE_END(); // options/subscript_operator

BOOST_AUTO_TEST_SUITE(stream_insertion_operator);

BOOST_AUTO_TEST_CASE(default_settings)
{
    cli::options options {
        cli::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        cli::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        cli::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        cli::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        cli::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        cli::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    options.description("general options:");

    std::ostringstream ostringstream;

    ostringstream << options;

    std::string_view output =
        "general options:\n"
        "\n"
        "    -e, --edit    edits files\n"
        "        --no-edit doesn't edit files\n"
        "    -f, --file    add input file\n"
        "    -h, --help    displays a help message\n"
        "    -t            set a timer for replying\n"
        "    -v, --verbose enables verbose mode\n";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_CASE(no_description)
{
    const cli::options options {
        cli::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        cli::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        cli::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        cli::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        cli::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        cli::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    std::ostringstream ostringstream;

    ostringstream << options;

    std::string_view output =
        "    -e, --edit    edits files\n"
        "        --no-edit doesn't edit files\n"
        "    -f, --file    add input file\n"
        "    -h, --help    displays a help message\n"
        "    -t            set a timer for replying\n"
        "    -v, --verbose enables verbose mode\n";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_CASE(vertical_gap_is_zero)
{
    cli::options::vertical_gap = 0;

    cli::options options {
        cli::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        cli::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        cli::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        cli::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        cli::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        cli::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    options.description("general options:");

    options.vertical_gap = 0;

    std::ostringstream ostringstream;

    ostringstream << options;

    std::string_view output =
        "general options:\n"
        "    -e, --edit    edits files\n"
        "        --no-edit doesn't edit files\n"
        "    -f, --file    add input file\n"
        "    -h, --help    displays a help message\n"
        "    -t            set a timer for replying\n"
        "    -v, --verbose enables verbose mode\n";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_CASE(left_indent_and_vertical_gap_are_zero)
{
    cli::options::left_indent = 0;

    cli::options::vertical_gap = 0;

    cli::options options {
        cli::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        cli::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        cli::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        cli::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        cli::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        cli::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    options.description("general options:");

    std::ostringstream ostringstream;

    ostringstream << options;

    std::string_view output =
        "general options:\n"
        "-e, --edit    edits files\n"
        "    --no-edit doesn't edit files\n"
        "-f, --file    add input file\n"
        "-h, --help    displays a help message\n"
        "-t            set a timer for replying\n"
        "-v, --verbose enables verbose mode\n";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_SUITE_END(); // options/stream_insertion_operator

BOOST_AUTO_TEST_SUITE_END(); // options
