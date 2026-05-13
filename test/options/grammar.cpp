#define BOOST_TEST_MODULE grammar

#define BOOST_TEST_DYN_LINK

#include <string_view>
#include <stdexcept>
#include <sstream>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "options/grammar.hpp"
#include "options/option.hpp"

BOOST_AUTO_TEST_SUITE(grammar);

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(default_constructor)
{
    options::grammar grammar;

    BOOST_TEST(not grammar.contains("-h"));

    BOOST_TEST(grammar.description().empty());

    constexpr auto predicate = [](const auto& exception)
    {
        return std::string_view(exception.what()) ==
            "grammar::operator[]: the -h option name is not saved";
    };

    BOOST_CHECK_EXCEPTION(grammar["-h"], std::logic_error, predicate);

    options::grammar const_grammar;

    BOOST_TEST(not const_grammar.contains("-h"));

    BOOST_TEST(const_grammar.description().empty());

    BOOST_CHECK_EXCEPTION(const_grammar["-h"], std::logic_error, predicate);
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
    const options::grammar grammar {
        options::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        options::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "display a help message"
        },

        options::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enable verbose mode"
        }
    };

    BOOST_TEST(grammar.description().empty());

    BOOST_TEST(grammar.contains("-f"));

    BOOST_CHECK_EQUAL(grammar["-f"].short_name, "-f");
    BOOST_CHECK_EQUAL(grammar["-f"].long_name, "--file");
    BOOST_CHECK_EQUAL(grammar["-f"].description, "add input file");

    BOOST_TEST(grammar.contains("--file"));

    BOOST_CHECK_EQUAL(grammar["--file"].short_name, "-f");
    BOOST_CHECK_EQUAL(grammar["--file"].long_name, "--file");
    BOOST_CHECK_EQUAL(grammar["--file"].description, "add input file");

    BOOST_TEST(grammar.contains("-h"));

    BOOST_CHECK_EQUAL(grammar["-h"].short_name, "-h");
    BOOST_CHECK_EQUAL(grammar["-h"].long_name, "--help");
    BOOST_CHECK_EQUAL(grammar["-h"].description, "display a help message");

    BOOST_TEST(grammar.contains("--help"));

    BOOST_CHECK_EQUAL(grammar["--help"].short_name, "-h");
    BOOST_CHECK_EQUAL(grammar["--help"].long_name, "--help");
    BOOST_CHECK_EQUAL(grammar["--help"].description, "display a help message");

    BOOST_TEST(grammar.contains("-v"));

    BOOST_CHECK_EQUAL(grammar["-v"].short_name, "-v");
    BOOST_CHECK_EQUAL(grammar["-v"].long_name, "--verbose");
    BOOST_CHECK_EQUAL(grammar["-v"].description, "enable verbose mode");

    BOOST_TEST(grammar.contains("--verbose"));

    BOOST_CHECK_EQUAL(grammar["--verbose"].short_name, "-v");
    BOOST_CHECK_EQUAL(grammar["--verbose"].long_name, "--verbose");
    BOOST_CHECK_EQUAL(grammar["--verbose"].description, "enable verbose mode");
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    const options::grammar grammar_1 {
        options::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        options::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "display a help message"
        },

        options::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enable verbose mode"
        }
    };

    const options::grammar grammar_2 {grammar_1};

    BOOST_TEST(grammar_2.description().empty());

    BOOST_TEST(grammar_2.contains("-f"));

    BOOST_CHECK_EQUAL(grammar_2["-f"].short_name, "-f");
    BOOST_CHECK_EQUAL(grammar_2["-f"].long_name, "--file");
    BOOST_CHECK_EQUAL(grammar_2["-f"].description, "add input file");

    BOOST_TEST(grammar_2.contains("--file"));

    BOOST_CHECK_EQUAL(grammar_2["--file"].short_name, "-f");
    BOOST_CHECK_EQUAL(grammar_2["--file"].long_name, "--file");
    BOOST_CHECK_EQUAL(grammar_2["--file"].description, "add input file");

    BOOST_TEST(grammar_2.contains("-h"));

    BOOST_CHECK_EQUAL(grammar_2["-h"].short_name, "-h");
    BOOST_CHECK_EQUAL(grammar_2["-h"].long_name, "--help");
    BOOST_CHECK_EQUAL(grammar_2["-h"].description, "display a help message");

    BOOST_TEST(grammar_2.contains("--help"));

    BOOST_CHECK_EQUAL(grammar_2["--help"].short_name, "-h");
    BOOST_CHECK_EQUAL(grammar_2["--help"].long_name, "--help");

    BOOST_CHECK_EQUAL(
        grammar_2["--help"].description, "display a help message");

    BOOST_TEST(grammar_2.contains("-v"));

    BOOST_CHECK_EQUAL(grammar_2["-v"].short_name, "-v");
    BOOST_CHECK_EQUAL(grammar_2["-v"].long_name, "--verbose");
    BOOST_CHECK_EQUAL(grammar_2["-v"].description, "enable verbose mode");

    BOOST_TEST(grammar_2.contains("--verbose"));

    BOOST_CHECK_EQUAL(grammar_2["--verbose"].short_name, "-v");
    BOOST_CHECK_EQUAL(grammar_2["--verbose"].long_name, "--verbose");
    BOOST_CHECK_EQUAL(grammar_2["--verbose"].description, "enable verbose mode");
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    options::grammar grammar_1 {
        options::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        options::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "display a help message"
        },

        options::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enable verbose mode"
        }
    };

    const options::grammar grammar_2 {std::move(grammar_1)};

    BOOST_TEST(grammar_1.empty());

    BOOST_TEST(not grammar_1.contains("-f"));
    BOOST_TEST(not grammar_1.contains("--file"));

    BOOST_TEST(not grammar_1.contains("-h"));
    BOOST_TEST(not grammar_1.contains("--help"));

    BOOST_TEST(not grammar_1.contains("-v"));
    BOOST_TEST(not grammar_1.contains("--verbose"));

    BOOST_CHECK_EQUAL(grammar_2.size(), 3);

    BOOST_TEST(grammar_2.description().empty());

    BOOST_TEST(grammar_2.contains("-f"));

    BOOST_CHECK_EQUAL(grammar_2["-f"].short_name, "-f");
    BOOST_CHECK_EQUAL(grammar_2["-f"].long_name, "--file");
    BOOST_CHECK_EQUAL(grammar_2["-f"].description, "add input file");

    BOOST_TEST(grammar_2.contains("--file"));

    BOOST_CHECK_EQUAL(grammar_2["--file"].short_name, "-f");
    BOOST_CHECK_EQUAL(grammar_2["--file"].long_name, "--file");
    BOOST_CHECK_EQUAL(grammar_2["--file"].description, "add input file");

    BOOST_TEST(grammar_2.contains("-h"));

    BOOST_CHECK_EQUAL(grammar_2["-h"].short_name, "-h");
    BOOST_CHECK_EQUAL(grammar_2["-h"].long_name, "--help");
    BOOST_CHECK_EQUAL(grammar_2["-h"].description, "display a help message");

    BOOST_TEST(grammar_2.contains("--help"));

    BOOST_CHECK_EQUAL(grammar_2["--help"].short_name, "-h");
    BOOST_CHECK_EQUAL(grammar_2["--help"].long_name, "--help");

    BOOST_CHECK_EQUAL(
        grammar_2["--help"].description, "display a help message");

    BOOST_TEST(grammar_2.contains("-v"));

    BOOST_CHECK_EQUAL(grammar_2["-v"].short_name, "-v");
    BOOST_CHECK_EQUAL(grammar_2["-v"].long_name, "--verbose");
    BOOST_CHECK_EQUAL(grammar_2["-v"].description, "enable verbose mode");

    BOOST_TEST(grammar_2.contains("--verbose"));

    BOOST_CHECK_EQUAL(grammar_2["--verbose"].short_name, "-v");
    BOOST_CHECK_EQUAL(grammar_2["--verbose"].long_name, "--verbose");
    BOOST_CHECK_EQUAL(grammar_2["--verbose"].description, "enable verbose mode");
}

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    const options::grammar const_grammar {"general options"};

    BOOST_CHECK_EQUAL(const_grammar.description(), "general options");

    BOOST_TEST(const_grammar.empty());

    BOOST_CHECK_EQUAL(const_grammar.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END(); // grammar/constructor

BOOST_AUTO_TEST_SUITE(add_option);

BOOST_AUTO_TEST_CASE(add_option_without_names)
{
    BOOST_CHECK_EXCEPTION(
        options::grammar().add_option(options::option()),
        std::logic_error,
        [](const auto& exception)
        {
            return std::string_view(exception.what()) ==
                "grammar::verify: "
                "the option must have at least a short or long name";
        });
}

BOOST_AUTO_TEST_CASE(add_option_with_short_name)
{
    const options::option help {
        .short_name  = "-h",
        .description = "display a help message"
    };

    options::grammar grammar;

    BOOST_CHECK_NO_THROW(grammar.add_option(help));

    BOOST_TEST(grammar.contains("-h"));

    BOOST_CHECK_EQUAL(grammar["-h"].short_name, "-h");

    BOOST_TEST(grammar["-h"].long_name.empty());

    BOOST_CHECK_EQUAL(grammar["-h"].description, "display a help message");
    BOOST_CHECK_EQUAL(grammar["-h"].is_required, false);
    BOOST_CHECK_EQUAL(grammar["-h"].has_arguments, false);

    BOOST_CHECK_EXCEPTION(
        grammar.add_option(options::option {"-h"}),
        std::logic_error,
        [](const auto& exception)
        {
            return std::string_view(exception.what()) ==
                "grammar::add_option: "
                "option [-h] has already been added as [-h]";
        });

    BOOST_CHECK_EXCEPTION(
        grammar.add_option(options::option {"-h", "--help"}),
        std::logic_error,
        [](const auto& exception)
        {
            return std::string_view(exception.what()) ==
                "grammar::add_option: "
                "option [-h, --help] has already been added as [-h]";
        });
}

BOOST_AUTO_TEST_CASE(add_option_with_long_name)
{
    const options::option help {
        .long_name   = "--help",
        .description = "display a help message"
    };

    options::grammar grammar;

    BOOST_CHECK_NO_THROW(grammar.add_option(help));

    BOOST_TEST(grammar.contains("--help"));
    BOOST_TEST(grammar["--help"].short_name.empty());

    BOOST_CHECK_EQUAL(grammar["--help"].long_name, "--help");
    BOOST_CHECK_EQUAL(grammar["--help"].description, "display a help message");
    BOOST_CHECK_EQUAL(grammar["--help"].is_required, false);
    BOOST_CHECK_EQUAL(grammar["--help"].has_arguments, false);

    BOOST_CHECK_EXCEPTION(
        grammar.add_option(options::option {.long_name = "--help"}),
        std::logic_error,
        [](const auto& exception)
        {
            return std::string_view(exception.what()) ==
                "grammar::add_option: "
                "option [--help] has already been added as [--help]";
        });

    BOOST_CHECK_EXCEPTION(
        grammar.add_option(options::option {"-h", "--help"}),
        std::logic_error,
        [](const auto& exception)
        {
            return std::string_view(exception.what()) ==
                "grammar::add_option: "
                "option [-h, --help] has already been added as [--help]";
        });
}

BOOST_AUTO_TEST_SUITE_END(); // grammar/add_option

BOOST_AUTO_TEST_SUITE(verify);

BOOST_AUTO_TEST_CASE(empty_options_names)
{
    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option()),
        std::logic_error,
        [](const auto& exception)
        {
            return std::string_view(exception.what()) ==
                "grammar::verify: "
                "the option must have at least a short or long name";
        });
}

BOOST_AUTO_TEST_CASE(invalid_long_option_names)
{
    constexpr auto predicate = [](const auto& exception)
    {
        return std::string_view(exception.what()) ==
            "grammar::verify: "
            "the long option name must match the pattern "
            "^--[a-zA-Z][-a-zA-Z0-9]*[a-zA-Z]$";
    };

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "abcd"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "--a"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "--A"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "--0"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "----"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "--0-"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "--!-"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "--a--"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.long_name = "--a-b-"}),
        std::logic_error,
        predicate);
}

BOOST_AUTO_TEST_CASE(invalid_short_option_names)
{
    constexpr auto predicate = [](const auto& exception)
    {
        return std::string_view(exception.what()) ==
            "grammar::verify: "
            "the short option name must match the pattern ^-[a-zA-Z0-9]$";
    };

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.short_name = " "}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.short_name = "-"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.short_name = "a"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.short_name = "A"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.short_name = "0"}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.short_name = "- "}),
        std::logic_error,
        predicate);

    BOOST_CHECK_EXCEPTION(
        options::grammar::verify(options::option {.short_name = "-!"}),
        std::logic_error,
        predicate);
}

BOOST_AUTO_TEST_CASE(valid_long_option_names)
{
    BOOST_CHECK_NO_THROW(
        options::grammar::verify(options::option {.long_name = "--ab"}));

    BOOST_CHECK_NO_THROW(
        options::grammar::verify(options::option {.long_name = "--Aa"}));

    BOOST_CHECK_NO_THROW(
        options::grammar::verify(options::option {.long_name = "--A-a"}));

    BOOST_CHECK_NO_THROW(
        options::grammar::verify(options::option {.long_name = "--a-0-b-1"}));
}

BOOST_AUTO_TEST_CASE(valid_short_option_names)
{
    BOOST_CHECK_NO_THROW(
        options::grammar::verify(options::option {.short_name = "-a"}));

    BOOST_CHECK_NO_THROW(
        options::grammar::verify(options::option {.short_name = "-A"}));

    BOOST_CHECK_NO_THROW(
        options::grammar::verify(options::option {.short_name = "-0"}));
}

BOOST_AUTO_TEST_SUITE_END(); // grammar/verify

BOOST_AUTO_TEST_SUITE(stream_insertion_operator);

BOOST_AUTO_TEST_CASE(default_settings)
{
    options::grammar grammar {
        options::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        options::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        options::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        options::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        options::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        options::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    grammar.description("general options:");

    std::ostringstream ostringstream;

    ostringstream << grammar;

    std::string_view output =
        "general options:\n"
        "\n"
        "    -e, --edit    edits files\n"
        "        --no-edit doesn't edit files\n"
        "    -f, --file    add input file\n"
        "    -h, --help    displays a help message\n"
        "    -t            set a timer for replying\n"
        "    -v, --verbose enables verbose mode";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_CASE(no_description)
{
    const options::grammar grammar {
        options::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        options::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        options::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        options::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        options::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        options::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    std::ostringstream ostringstream;

    ostringstream << grammar;

    std::string_view output =
        "    -e, --edit    edits files\n"
        "        --no-edit doesn't edit files\n"
        "    -f, --file    add input file\n"
        "    -h, --help    displays a help message\n"
        "    -t            set a timer for replying\n"
        "    -v, --verbose enables verbose mode";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_CASE(vertical_gap_is_zero)
{
    options::grammar grammar {
        options::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        options::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        options::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        options::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        options::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        options::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    grammar.description("general options:");

    grammar.vertical_gap = 0;

    std::ostringstream ostringstream;

    ostringstream << grammar;

    std::string_view output =
        "general options:\n"
        "    -e, --edit    edits files\n"
        "        --no-edit doesn't edit files\n"
        "    -f, --file    add input file\n"
        "    -h, --help    displays a help message\n"
        "    -t            set a timer for replying\n"
        "    -v, --verbose enables verbose mode";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_CASE(vertical_gap_and_left_indent_are_zero)
{
    options::grammar grammar {
        options::option {
            .short_name  = "-e",
            .long_name   = "--edit",
            .description = "edits files"
        },

        options::option {
            .long_name   = "--no-edit",
            .description = "doesn't edit files"
        },

        options::option {
            .short_name  = "-f",
            .long_name   = "--file",
            .description = "add input file"
        },

        options::option {
            .short_name  = "-h",
            .long_name   = "--help",
            .description = "displays a help message"
        },

        options::option {
            .short_name  = "-t",
            .description = "set a timer for replying"
        },

        options::option {
            .short_name  = "-v",
            .long_name   = "--verbose",
            .description = "enables verbose mode"
        }
    };

    grammar.description("general options:");

    grammar.left_indent  = 0;
    grammar.vertical_gap = 0;

    std::ostringstream ostringstream;

    ostringstream << grammar;

    std::string_view output =
        "general options:\n"
        "-e, --edit    edits files\n"
        "    --no-edit doesn't edit files\n"
        "-f, --file    add input file\n"
        "-h, --help    displays a help message\n"
        "-t            set a timer for replying\n"
        "-v, --verbose enables verbose mode";

    BOOST_CHECK_EQUAL(ostringstream.view(), output);
}

BOOST_AUTO_TEST_SUITE_END(); // grammar/stream_insertion_operator

BOOST_AUTO_TEST_SUITE_END(); // grammar
