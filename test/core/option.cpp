#define BOOST_TEST_MODULE option

// option.cpp must be compiled with DEBUG
// to correctly test short_name(), long_name()

#include <utility>

#include <boost/test/unit_test.hpp>

#include "core/option.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(initalization);

BOOST_AUTO_TEST_CASE(default_initilization)
{
    const option option;

    BOOST_TEST(option.short_name().empty());
    BOOST_TEST(option.long_name().empty());
    BOOST_TEST(option.output_representation().empty());
    BOOST_TEST(option.description().empty());

    BOOST_CHECK_EQUAL(option.is_required(),            false);
    BOOST_CHECK_EQUAL(option.has_arguments(),          false);
    BOOST_CHECK_EQUAL(option.has_equality_validator(), false);
}

BOOST_AUTO_TEST_CASE(initialize_with_invalid_short_name)
{
    BOOST_TEST(option("").short_name().empty());
    BOOST_TEST(option("-").short_name().empty());
    BOOST_TEST(option("--help").short_name().empty());
}

BOOST_AUTO_TEST_CASE(initialize_with_invalid_long_name)
{
    BOOST_TEST(option({}, "-h").long_name().empty());
    BOOST_TEST(option({}, "-help").long_name().empty());
}

BOOST_AUTO_TEST_CASE(full_initialization)
{
    const option help {
	"-h",
	"--help",
	"-h, --help",
	"produce help message",
	option::required::not_required,
	option::arguments::no_arguments,
	{}
    };

    BOOST_CHECK_EQUAL(help.short_name().empty(),            false);
    BOOST_CHECK_EQUAL(help.long_name().empty(),             false);
    BOOST_CHECK_EQUAL(help.output_representation().empty(), false);
    BOOST_CHECK_EQUAL(help.description().empty(),           false);
    BOOST_CHECK_EQUAL(help.is_required(),                   false);
    BOOST_CHECK_EQUAL(help.has_arguments(),                 false);
    BOOST_CHECK_EQUAL(help.has_equality_validator(),        false);
}

BOOST_AUTO_TEST_CASE(copy_initialization)
{
    const option help {
	"-h",
	"--help",
	"-h, --help",
	"produce help message",
	option::required::not_required,
	option::arguments::no_arguments,
	{}
    };

    BOOST_CHECK_EQUAL(help.short_name().empty(),            false);
    BOOST_CHECK_EQUAL(help.long_name().empty(),             false);
    BOOST_CHECK_EQUAL(help.output_representation().empty(), false);
    BOOST_CHECK_EQUAL(help.description().empty(),           false);
    BOOST_CHECK_EQUAL(help.is_required(),                   false);
    BOOST_CHECK_EQUAL(help.has_arguments(),                 false);
    BOOST_CHECK_EQUAL(help.has_equality_validator(),        false);

    const option option {help};

    BOOST_CHECK_EQUAL(option.short_name().empty(),            false);
    BOOST_CHECK_EQUAL(option.long_name().empty(),             false);
    BOOST_CHECK_EQUAL(option.output_representation().empty(), false);
    BOOST_CHECK_EQUAL(option.description().empty(),           false);
    BOOST_CHECK_EQUAL(option.is_required(),                   false);
    BOOST_CHECK_EQUAL(option.has_arguments(),                 false);
    BOOST_CHECK_EQUAL(option.has_equality_validator(),        false);
}

BOOST_AUTO_TEST_CASE(move_initialization)
{
    option help {
	"-h",
	"--help",
	"-h, --help",
	"produce help message",
	option::required::not_required,
	option::arguments::no_arguments,
	{}
    };

    const option option {std::move(help)};

    BOOST_CHECK_EQUAL(option.short_name().empty(),            false);
    BOOST_CHECK_EQUAL(option.long_name().empty(),             false);
    BOOST_CHECK_EQUAL(option.output_representation().empty(), false);
    BOOST_CHECK_EQUAL(option.description().empty(),           false);
    BOOST_CHECK_EQUAL(option.is_required(),                   false);
    BOOST_CHECK_EQUAL(option.has_arguments(),                 false);
    BOOST_CHECK_EQUAL(option.has_equality_validator(),        false);

    BOOST_TEST(help.short_name().empty());
    BOOST_TEST(help.long_name().empty());
    BOOST_TEST(help.output_representation().empty());
    BOOST_TEST(help.description().empty());

    BOOST_CHECK_EQUAL(help.is_required(),            false);
    BOOST_CHECK_EQUAL(help.has_arguments(),          false);
    BOOST_CHECK_EQUAL(help.has_equality_validator(), false);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(short_name);

BOOST_AUTO_TEST_CASE(assign_short_name)
{
    option help;

    help.short_name("-h");

    BOOST_CHECK_EQUAL(help.short_name(), "-h");
}

BOOST_AUTO_TEST_CASE(assign_invalid_short_name)
{
    option help;

    help.short_name("-");

    BOOST_TEST(help.short_name().empty());

    help.short_name("--help");

    BOOST_TEST(help.short_name().empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(long_name);

BOOST_AUTO_TEST_CASE(assign_long_name)
{
    option help;

    help.long_name("--help");

    BOOST_CHECK_EQUAL(help.long_name(), "--help");
}

BOOST_AUTO_TEST_CASE(assign_invalid_long_name)
{
    option help;

    help.long_name("-h");

    BOOST_TEST(help.long_name().empty());

    help.long_name("-help");

    BOOST_TEST(help.long_name().empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_option_name);

BOOST_AUTO_TEST_CASE(check_valid_short_option_name)
{
    BOOST_TEST(option::is_short_option_name("-a"));
    BOOST_TEST(option::is_short_option_name("-Z"));
    BOOST_TEST(option::is_short_option_name("-1"));
    BOOST_TEST(option::is_short_option_name("-!"));
    BOOST_TEST(option::is_short_option_name("--"));
    BOOST_TEST(option::is_short_option_name("- "));
}

BOOST_AUTO_TEST_CASE(check_invalid_short_option_name)
{
    BOOST_TEST(not option::is_short_option_name(""));
    BOOST_TEST(not option::is_short_option_name("-"));
    BOOST_TEST(not option::is_short_option_name("a"));
    BOOST_TEST(not option::is_short_option_name("---"));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_long_option_name);

BOOST_AUTO_TEST_CASE(check_valid_long_option_name)
{
    BOOST_TEST(option::is_long_option_name("-- "));
    BOOST_TEST(option::is_long_option_name("---"));
    BOOST_TEST(option::is_long_option_name("--="));
    BOOST_TEST(option::is_long_option_name("--a"));
    BOOST_TEST(option::is_long_option_name("--help"));
    BOOST_TEST(option::is_long_option_name("--file="));
}

BOOST_AUTO_TEST_CASE(check_invalid_long_option_name)
{
    BOOST_TEST(not option::is_long_option_name(""));
    BOOST_TEST(not option::is_long_option_name("--"));
    BOOST_TEST(not option::is_long_option_name("-a"));
    BOOST_TEST(not option::is_long_option_name("help"));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_long_option_name_with_argument);

BOOST_AUTO_TEST_CASE(check_valid_long_option_name_with_argument)
{
    BOOST_TEST(option::is_long_option_name_with_argument("--="));
    BOOST_TEST(option::is_long_option_name_with_argument("-- file="));
    BOOST_TEST(option::is_long_option_name_with_argument("--count=5"));
    BOOST_TEST(option::is_long_option_name_with_argument("--option="));
    BOOST_TEST(option::is_long_option_name_with_argument("--special=!@#$%^&*()"));
    BOOST_TEST(option::is_long_option_name_with_argument("---file=test.txt"));
}

BOOST_AUTO_TEST_CASE(check_invalid_long_option_name_with_argument)
{
    BOOST_TEST(not option::is_long_option_name_with_argument(""));
    BOOST_TEST(not option::is_long_option_name_with_argument("-"));
    BOOST_TEST(not option::is_long_option_name_with_argument("--"));
    BOOST_TEST(not option::is_long_option_name_with_argument("--file"));
}

BOOST_AUTO_TEST_SUITE_END();
