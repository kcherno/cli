#define BOOST_TEST_MODULE parser

#define BOOST_TEST_DYN_LINK

#include <string_view>
#include <stdexcept>
#include <string>

#include <boost/test/unit_test.hpp>

#include "options/grammar.hpp"
#include "options/option.hpp"
#include "options/parser.hpp"

BOOST_AUTO_TEST_SUITE(parser);

BOOST_AUTO_TEST_SUITE(parsed_options);

BOOST_AUTO_TEST_SUITE(subscript_operator);

BOOST_AUTO_TEST_CASE(option_is_not_stored)
{
    const char* argv[] = {"<application name>", nullptr};

    constexpr int argc = 1;

    const auto& [parsed_options, positional_options] =
        options::parser().parse_command_line(argc, argv);

    BOOST_CHECK_EXCEPTION(
        parsed_options["-h"], std::runtime_error, [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parsed_options::operator[]: -h is not stored";
        });
}

BOOST_AUTO_TEST_CASE(option_has_no_arguments)
{
    const options::grammar grammar {
        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        }
    };

    const char* argv[] = {"<application name>", "-h", nullptr};

    constexpr int argc = 2;

    const auto& [parsed_options, positional_options] =
        options::parser(grammar).parse_command_line(argc, argv);

    BOOST_CHECK_EXCEPTION(
        parsed_options["-h"], std::logic_error, [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parsed_options::operator[]: "
                "[-h, --help] has no arguments";
        });
}

BOOST_AUTO_TEST_SUITE_END(); // parser/parsed_options/subscript_operator

BOOST_AUTO_TEST_SUITE_END(); // parser/parsed_options

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(default_constructor)
{
    const char* argv[] = {
        "<application name>",
        "--help",
        nullptr
    };

    constexpr int argc = 2;

    BOOST_CHECK_EXCEPTION(
        options::parser().parse_command_line(argc, argv),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: --help is an unrecognized option";
        });
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    const options::grammar grammar {
        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        }
    };

    const options::parser parser {grammar};

    const char* argv[] = {
        "<application name>",
        "--help",
        nullptr
    };

    constexpr int argc = 2;

    BOOST_CHECK_NO_THROW(
        options::parser(parser).parse_command_line(argc, argv));
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    const options::grammar grammar {
        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        }
    };

    options::parser parser {grammar};

    const char* argv[] = {
        "<application name>",
        "--help",
        nullptr
    };

    constexpr int argc = 2;

    const options::parser const_parser {std::move(parser)};

    BOOST_CHECK_EXCEPTION(
        parser.parse_command_line(argc, argv),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: --help is an unrecognized option";
        });
}

BOOST_AUTO_TEST_SUITE(parameterized_constructor);

BOOST_AUTO_TEST_CASE(initializer_list)
{
    const options::parser parser {
        options::grammar {
            options::option {
                .short_name = "-f",
                .long_name  = "--file",
            }
        },

        options::grammar {
            options::option {
                .short_name  = "-m",
                .long_name   = "--mode",
                .is_required = true
            }
        }
    };

    const char* argv[] = {
        "<application name>",
        "--mode=soft",
        "-f",
        "file.txt",
        nullptr
    };

    constexpr int argc = 4;

    BOOST_CHECK_NO_THROW(parser.parse_command_line(argc, argv));
}

BOOST_AUTO_TEST_SUITE_END(); // parser/constructor/parameterized_constructor

BOOST_AUTO_TEST_SUITE_END(); // parser/constructor

BOOST_AUTO_TEST_SUITE(assignment_operator);

BOOST_AUTO_TEST_CASE(copy)
{
    const options::grammar grammar {
        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        }
    };

    const options::parser parser_1 {grammar};

    options::parser parser_2;

    parser_2 = parser_1;

    const char* argv[] = {
        "<application name>",
        "--help",
        nullptr
    };

    constexpr int argc = 2;

    BOOST_CHECK_NO_THROW(parser_2.parse_command_line(argc, argv));
}

BOOST_AUTO_TEST_CASE(move)
{
    const options::grammar grammar {
        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        }
    };

    options::parser parser_1 {grammar};

    options::parser parser_2;

    parser_2 = std::move(parser_1);

    const char* argv[] = {
        "<application name>",
        "--help",
        nullptr
    };

    constexpr int argc = 2;

    BOOST_CHECK_EXCEPTION(
        parser_1.parse_command_line(argc, argv),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: --help is an unrecognized option";
        });
}

BOOST_AUTO_TEST_SUITE_END(); // parser/assignment_operator

BOOST_AUTO_TEST_SUITE(parse_command_line);

BOOST_AUTO_TEST_CASE(parse_empty_command_line)
{
    const char* argv[] = {"<application name>", nullptr};

    constexpr int argc = 1;

    const auto& [parsed_options, positional_options] =
        options::parser().parse_command_line(argc, argv);

    BOOST_TEST(parsed_options.empty());

    BOOST_TEST(positional_options.empty());
}

BOOST_AUTO_TEST_CASE(unrecognized_option)
{
    const options::grammar grammar {
        options::option {
            .short_name    = "-e",
            .long_name     = "--edit",
            .has_arguments = true
        },

        options::option {
            .long_name     = "--no-edit",
            .has_arguments = true
        },

        options::option {
            .short_name    = "-f",
            .long_name     = "--file",
            .has_arguments = true
        },

        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        },

        options::option {
            .short_name = "-v",
            .long_name  = "--verbose"
        }
    };

    const char* argv[] = {"<application name>", "--mode", nullptr};

    constexpr int argc = 2;

    BOOST_CHECK_EXCEPTION(
        options::parser(grammar).parse_command_line(argc, argv),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: --mode is an unrecognized option";
        });
}

BOOST_AUTO_TEST_CASE(option_expects_an_argument)
{
    const options::grammar grammar {
        options::option {
            .short_name    = "-e",
            .long_name     = "--edit",
            .has_arguments = true
        },

        options::option {
            .long_name     = "--no-edit",
            .has_arguments = true
        },

        options::option {
            .short_name    = "-f",
            .long_name     = "--file",
            .has_arguments = true
        },

        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        },

        options::option {
            .short_name = "-v",
            .long_name  = "--verbose"
        }
    };

    const char* argv_1[] = {"<application name>", "-f", nullptr};

    constexpr int argc_1 = 2;

    BOOST_CHECK_EXCEPTION(
        options::parser(grammar).parse_command_line(argc_1, argv_1),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: -f expects an argument";
        });

    const char* argv_2[] = {"<application name>", "--file", nullptr};

    constexpr int argc_2 = 2;

    BOOST_CHECK_EXCEPTION(
        options::parser(grammar).parse_command_line(argc_2, argv_2),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: --file expects an argument";
        });

    const char* argv_3[] = {"<application name>", "--file=", nullptr};

    constexpr int argc_3 = 2;

    BOOST_CHECK_EXCEPTION(
        options::parser(grammar).parse_command_line(argc_3, argv_3),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: --file expects an argument";
        });
}

BOOST_AUTO_TEST_CASE(option_arguments_are_not_required)
{
    const options::grammar grammar {
        options::option {
            .short_name    = "-f",
            .long_name     = "--file",
            .has_arguments = true
        },

        options::option {
            .short_name             = "-h",
            .long_name              = "--help",
            .has_arguments          = true,
            .are_arguments_required = false
        }
    };

    const char* argv[] = {
        "<application name>",
        "--help",
        "a",
        "-f",
        "file_1.txt",
        "-h",
        "--file=file_2.txt",
        nullptr
    };

    constexpr int argc = 2;

    BOOST_CHECK_NO_THROW(
        options::parser(grammar).parse_command_line(argc, argv));

    const auto& [parsed_options, positional_options] =
        options::parser(grammar).parse_command_line(argc, argv);

    BOOST_CHECK_NO_THROW(parsed_options["-h"]);

    BOOST_CHECK_EQUAL(parsed_options["-h"].size(), 1);
    BOOST_CHECK_EQUAL(parsed_options["-h"][0], "a");

    BOOST_CHECK_NO_THROW(parsed_options["--help"]);

    BOOST_CHECK_EQUAL(parsed_options["--help"].size(), 1);
    BOOST_CHECK_EQUAL(parsed_options["--help"][0], "a");
}

BOOST_AUTO_TEST_CASE(invalid_argument_for_option)
{
    const options::grammar grammar {
        options::option {
            .short_name        = "-m",
            .long_name         = "--mode",
            .is_required       = true,
            .has_arguments     = true,
            .argument_verifier = [](std::string_view argument)
            {
                if (argument != "soft" || argument != "hard")
                {
                    throw std::logic_error {
                        std::string("option::argument_verifier: [-m, --mode] ")
                            .append("expects only arguments 'soft' or 'hard'")
                    };
                }
            }
        }
    };

    const char* argv[] = {
        "<application name>",
        "-m",
        "cool",
        nullptr
    };

    constexpr auto argc = 3;

    BOOST_CHECK_EXCEPTION(
        options::parser(grammar).parse_command_line(argc, argv),
        std::logic_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "option::argument_verifier: "
                "[-m, --mode] expects only arguments 'soft' or 'hard'";
        });
}

BOOST_AUTO_TEST_CASE(option_is_required)
{
    const options::grammar grammar {
        options::option {
            .short_name    = "-e",
            .long_name     = "--edit",
            .has_arguments = true
        },

        options::option {
            .long_name     = "--no-edit",
            .has_arguments = true
        },

        options::option {
            .short_name    = "-f",
            .long_name     = "--file",
            .has_arguments = true
        },

        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        },

        options::option {
            .short_name    = "-m",
            .long_name     = "--mode",
            .is_required   = true,
            .has_arguments = true
        },

        options::option {
            .short_name = "-v",
            .long_name  = "--verbose"
        }
    };

    const char* argv[] = {"<application name>", "--verbose", nullptr};

    constexpr int argc = 2;

    BOOST_CHECK_EXCEPTION(
        options::parser(grammar).parse_command_line(argc, argv),
        std::runtime_error,
        [](auto&& exception)
        {
            return std::string_view(exception.what()) ==
                "parser::parse_command_line: [-m, --mode] is required";
        });
}

BOOST_AUTO_TEST_CASE(correct_command_line_parsing)
{
    const options::grammar grammar {
        options::option {
            .short_name    = "-e",
            .long_name     = "--edit",
            .has_arguments = true
        },

        options::option {
            .long_name     = "--no-edit",
            .has_arguments = true
        },

        options::option {
            .short_name    = "-f",
            .long_name     = "--file",
            .has_arguments = true
        },

        options::option {
            .short_name = "-h",
            .long_name  = "--help"
        },

        options::option {
            .short_name    = "-m",
            .long_name     = "--mode",
            .is_required   = true,
            .has_arguments = true
        },

        options::option {
            .short_name = "-v",
            .long_name  = "--verbose"
        }
    };

    const char* argv[] = {
        "<application name>",
        "--mode=soft",
        "file_1.txt",
        "-e",
        "data_1.dat",
        "-f",
        "file_2.txt",
        "--no-edit=data_2.dat",
        "file_3.txt",
        "--file",
        "file_4.txt",
        "--edit=data_4.dat",
        "file_5.txt",
        "--file=file_6.txt",
        "--edit",
        "data_6.dat",
        "data_3.dat",
        "data_5.dat",
        nullptr
    };

    constexpr int argc = 18;

    const auto& [parsed_options, positional_options] =
        options::parser(grammar).parse_command_line(argc, argv);

    BOOST_CHECK_EQUAL(parsed_options.size(), 4);

    BOOST_TEST(parsed_options.contains("-e"));

    BOOST_REQUIRE_EQUAL(parsed_options["-e"].size(), 3);

    BOOST_CHECK_EQUAL(parsed_options["-e"][0], "data_1.dat");
    BOOST_CHECK_EQUAL(parsed_options["-e"][1], "data_4.dat");
    BOOST_CHECK_EQUAL(parsed_options["-e"][2], "data_6.dat");

    BOOST_TEST(parsed_options.contains("--edit"));

    BOOST_REQUIRE_EQUAL(parsed_options["--edit"].size(), 3);

    BOOST_CHECK_EQUAL(parsed_options["--edit"][0], "data_1.dat");
    BOOST_CHECK_EQUAL(parsed_options["--edit"][1], "data_4.dat");
    BOOST_CHECK_EQUAL(parsed_options["--edit"][2], "data_6.dat");

    BOOST_TEST(parsed_options.contains("--no-edit"));

    BOOST_REQUIRE_EQUAL(parsed_options["--no-edit"].size(), 1);

    BOOST_CHECK_EQUAL(parsed_options["--no-edit"][0], "data_2.dat");

    BOOST_TEST(parsed_options.contains("-f"));

    BOOST_REQUIRE_EQUAL(parsed_options["-f"].size(), 3);

    BOOST_CHECK_EQUAL(parsed_options["-f"][0], "file_2.txt");
    BOOST_CHECK_EQUAL(parsed_options["-f"][1], "file_4.txt");
    BOOST_CHECK_EQUAL(parsed_options["-f"][2], "file_6.txt");

    BOOST_TEST(parsed_options.contains("--file"));

    BOOST_REQUIRE_EQUAL(parsed_options["--file"].size(), 3);

    BOOST_CHECK_EQUAL(parsed_options["--file"][0], "file_2.txt");
    BOOST_CHECK_EQUAL(parsed_options["--file"][1], "file_4.txt");
    BOOST_CHECK_EQUAL(parsed_options["--file"][2], "file_6.txt");

    BOOST_TEST(parsed_options.contains("-m"));

    BOOST_REQUIRE_EQUAL(parsed_options["-m"].size(), 1);

    BOOST_CHECK_EQUAL(parsed_options["-m"][0], "soft");

    BOOST_TEST(parsed_options.contains("--mode"));

    BOOST_REQUIRE_EQUAL(parsed_options["--mode"].size(), 1);

    BOOST_CHECK_EQUAL(parsed_options["--mode"][0], "soft");

    BOOST_REQUIRE_EQUAL(positional_options.size(), 5);

    BOOST_CHECK_EQUAL(positional_options[0], "file_1.txt");
    BOOST_CHECK_EQUAL(positional_options[1], "file_3.txt");
    BOOST_CHECK_EQUAL(positional_options[2], "file_5.txt");
    BOOST_CHECK_EQUAL(positional_options[3], "data_3.dat");
    BOOST_CHECK_EQUAL(positional_options[4], "data_5.dat");
}

BOOST_AUTO_TEST_SUITE_END(); // parser/parse_command_line

BOOST_AUTO_TEST_SUITE_END(); // parser
