#define BOOST_TEST_MODULE parser

#include <boost/test/unit_test.hpp>

#include "core/parser.hpp"
#include "error/error.hpp"

BOOST_AUTO_TEST_SUITE(parse_command_line_options);

BOOST_AUTO_TEST_CASE(no_options)
{
    const char* argv[] = {"", "a.txt", "b.txt", nullptr};

    const auto options =
        cli::core::parser().parse_command_line_options(std::size(argv), argv);

    BOOST_TEST(options.empty());
}

BOOST_AUTO_TEST_CASE(valid_short_options)
{
    const char* argv[] = {"", "-a", "-b", nullptr};

    const auto options =
        cli::core::parser().parse_command_line_options(std::size(argv), argv);

    BOOST_CHECK_EQUAL(options.size(), 2);

    BOOST_CHECK_EQUAL(options[0], "-a");
    BOOST_CHECK_EQUAL(options[1], "-b");
}

BOOST_AUTO_TEST_CASE(valid_long_options)
{
    const char* argv[] = {"", "--first", "--second", nullptr};

    const auto options =
        cli::core::parser().parse_command_line_options(std::size(argv), argv);

    BOOST_CHECK_EQUAL(options.size(), 2);

    BOOST_CHECK_EQUAL(options[0], "--first");
    BOOST_CHECK_EQUAL(options[1], "--second");
}

BOOST_AUTO_TEST_CASE(mixed_options_and_arguments)
{
    const char* argv[] = {"", "-a", "file.txt", "--option", "data", nullptr};

    const auto options =
        cli::core::parser().parse_command_line_options(std::size(argv), argv);

    BOOST_CHECK_EQUAL(options.size(), 2);

    BOOST_CHECK_EQUAL(options[0], "-a");
    BOOST_CHECK_EQUAL(options[1], "--option");
}

BOOST_AUTO_TEST_CASE(short_option_empty_exception)
{
    const char* argv[] = {"", "-", nullptr};

    BOOST_CHECK_THROW(
        cli::core::parser().parse_command_line_options(std::size(argv), argv),
        cli::error::short_option_empty
    );
}

BOOST_AUTO_TEST_CASE(long_option_empty_exception)
{
    const char* argv[] = {"", "--", nullptr};

    BOOST_CHECK_THROW(
        cli::core::parser().parse_command_line_options(std::size(argv), argv),
        cli::error::long_option_empty
    );
}

BOOST_AUTO_TEST_CASE(long_option_with_one_hyphen_exception)
{
    const char* argv[] = {"", "-long-option", nullptr};

    BOOST_CHECK_THROW(
        cli::core::parser().parse_command_line_options(std::size(argv), argv),
        cli::error::long_option_with_one_hyphen
    );
}

BOOST_AUTO_TEST_CASE(long_option_starts_with_hyphen_exception)
{
    const char* argv[] = {"", "---option", nullptr};

    BOOST_CHECK_THROW(
        cli::core::parser().parse_command_line_options(std::size(argv), argv),
        cli::error::long_option_starts_with_hyphen
    );
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(parse_positional_options);

BOOST_AUTO_TEST_CASE(no_positional_options)
{
    const char* argv[] = {"", "-a", "--option", nullptr};
    
    const auto options =
	cli::core::parser().parse_positional_options(std::size(argv), argv);
    
    BOOST_TEST(options.empty());
}

BOOST_AUTO_TEST_CASE(valid_positional_options)
{
    const char* argv[] = {"", "a.txt", "b.txt", nullptr};
    
    const auto options =
	cli::core::parser().parse_positional_options(std::size(argv), argv);
    
    BOOST_CHECK_EQUAL(options.size(), 2);

    BOOST_CHECK_EQUAL(options[0], "a.txt");
    BOOST_CHECK_EQUAL(options[1], "b.txt");
}

BOOST_AUTO_TEST_CASE(mixed_options_and_positional_arguments)
{
    const char* argv[] = {"", "-a", "a.txt", "--option", "b.txt", nullptr};
    
    const auto options =
	cli::core::parser().parse_positional_options(std::size(argv), argv);
    
    BOOST_CHECK_EQUAL(options.size(), 2);

    BOOST_CHECK_EQUAL(options[0], "a.txt");
    BOOST_CHECK_EQUAL(options[1], "b.txt");
}

BOOST_AUTO_TEST_CASE(empty_arguments_skipped)
{
    const char* argv[] = {"", "", "a.txt", "", "b.txt", nullptr};

    const auto options =
	cli::core::parser().parse_positional_options(std::size(argv), argv);
    
    BOOST_CHECK_EQUAL(options.size(), 2);

    BOOST_CHECK_EQUAL(options[0], "a.txt");
    BOOST_CHECK_EQUAL(options[1], "b.txt");
}

BOOST_AUTO_TEST_SUITE_END();
