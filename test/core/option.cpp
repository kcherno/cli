#define BOOST_TEST_MODULE option

#include <string_view>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "core/option.hpp"

#include "error/option_must_have_at_least_short_or_long_name.hpp"
#include "error/invalid_format_for_short_option_name.hpp"
#include "error/invalid_format_for_long_option_name.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(default_constructor)
{
    BOOST_CHECK_THROW(
        option {}, cli::error::option_must_have_at_least_short_or_long_name);
}

BOOST_AUTO_TEST_SUITE(parameterized_constructor);

BOOST_AUTO_TEST_CASE(invalid_short_option_name)
{
    BOOST_CHECK_THROW(
        option {"-"}, cli::error::invalid_format_for_short_option_name);
}

BOOST_AUTO_TEST_CASE(valid_short_option_name)
{
    BOOST_CHECK_NO_THROW(option {"-h"});
}

BOOST_AUTO_TEST_CASE(invalid_long_option_name)
{
    BOOST_CHECK_THROW(
        option({}, "--"), cli::error::invalid_format_for_long_option_name);
}

BOOST_AUTO_TEST_CASE(valid_long_option_name)
{
    BOOST_CHECK_NO_THROW(option({}, "--help"));
}

BOOST_AUTO_TEST_CASE(full_initialization)
{
    const option verbose {
	"-v",
	"--verbose",
	"-v, --[no-]verbose",
	"verbose mode",
	option::required::not_required,
	option::arguments::no_arguments,
	[](auto&& option_name)
	{
	    return (option_name == "-v"        ||
		    option_name == "--verbose" ||
		    option_name == "--no-verbose");
	}
    };

    BOOST_CHECK_EQUAL(verbose.short_name(),     "-v");
    BOOST_CHECK_EQUAL(verbose.long_name(),      "--verbose");
    BOOST_CHECK_EQUAL(verbose.representation(), "-v, --[no-]verbose");
    BOOST_CHECK_EQUAL(verbose.description(),    "verbose mode");

    BOOST_TEST(not verbose.is_required());
    BOOST_TEST(not verbose.has_arguments());
    BOOST_TEST(verbose.has_equality_validator());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    const option verbose_1 {
	"-v",
	"--verbose",
	"-v, --[no-]verbose",
	"verbose mode",
	option::required::not_required,
	option::arguments::no_arguments,
	[](auto&& option_name)
	{
	    return (option_name == "-v"        ||
		    option_name == "--verbose" ||
		    option_name == "--no-verbose");
	}
    };

    const auto verbose_2 {verbose_1};

    BOOST_CHECK_EQUAL(verbose_2.short_name(),     verbose_1.short_name());
    BOOST_CHECK_EQUAL(verbose_2.long_name(),      verbose_1.long_name());
    BOOST_CHECK_EQUAL(verbose_2.representation(), verbose_1.representation());
    BOOST_CHECK_EQUAL(verbose_2.description(),    verbose_1.description());
    BOOST_CHECK_EQUAL(verbose_2.is_required(),    verbose_1.is_required());
    BOOST_CHECK_EQUAL(verbose_2.has_arguments(),  verbose_1.has_arguments());
    BOOST_CHECK_EQUAL(
        verbose_2.has_equality_validator(), verbose_1.has_equality_validator());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    option verbose_1 {
	"-v",
	"--verbose",
	"-v, --[no-]verbose",
	"verbose mode",
	option::required::not_required,
	option::arguments::no_arguments,
	[](auto&& option_name)
	{
	    return (option_name == "-v"        ||
		    option_name == "--verbose" ||
		    option_name == "--no-verbose");
	}
    };

    option verbose_2 {std::move(verbose_1)};

    BOOST_TEST(verbose_1.short_name().empty());
    BOOST_TEST(verbose_1.long_name().empty());
    BOOST_TEST(verbose_1.representation().empty());
    BOOST_TEST(verbose_1.description().empty());
    BOOST_TEST(not verbose_1.is_required());
    BOOST_TEST(not verbose_1.has_arguments());
    BOOST_TEST(not verbose_1.has_equality_validator());

    BOOST_CHECK_EQUAL(verbose_2.short_name(),     "-v");
    BOOST_CHECK_EQUAL(verbose_2.long_name(),      "--verbose");
    BOOST_CHECK_EQUAL(verbose_2.representation(), "-v, --[no-]verbose");
    BOOST_CHECK_EQUAL(verbose_2.description(),    "verbose mode");

    BOOST_TEST(not verbose_2.is_required());
    BOOST_TEST(not verbose_2.has_arguments());
    BOOST_TEST(verbose_2.has_equality_validator());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(short_name);

BOOST_AUTO_TEST_CASE(new_invalid_short_option_name)
{
    BOOST_CHECK_THROW(
        option("-h").short_name("-"),
	cli::error::invalid_format_for_short_option_name);

    BOOST_CHECK_THROW(
        option("-h").short_name("--help"),
	cli::error::invalid_format_for_short_option_name);
}

BOOST_AUTO_TEST_CASE(new_valid_short_option_name)
{
    BOOST_CHECK_NO_THROW(option("-h").short_name("- "));
    BOOST_CHECK_NO_THROW(option("-h").short_name("-v"));
    BOOST_CHECK_NO_THROW(option("-h").short_name("-1"));
    BOOST_CHECK_NO_THROW(option("-h").short_name("-#"));
}

BOOST_AUTO_TEST_CASE(remove_short_option_name)
{
    BOOST_CHECK_THROW(
        option("-h").short_name(""),
	cli::error::option_must_have_at_least_short_or_long_name);

    BOOST_CHECK_NO_THROW(option("-h", "--help").short_name(""));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(long_name);

BOOST_AUTO_TEST_CASE(new_invalid_long_name)
{
    BOOST_CHECK_THROW(
        option({}, "--help").long_name("--"),
	cli::error::invalid_format_for_long_option_name);

    BOOST_CHECK_THROW(
        option({}, "--help").long_name("-h"),
	cli::error::invalid_format_for_long_option_name);
}

BOOST_AUTO_TEST_CASE(new_valid_long_name)
{
    BOOST_CHECK_NO_THROW(option({}, "--help").long_name("-- "));
    BOOST_CHECK_NO_THROW(option({}, "--help").long_name("--v"));
    BOOST_CHECK_NO_THROW(option({}, "--help").long_name("--1"));
    BOOST_CHECK_NO_THROW(option({}, "--help").long_name("--#"));
}

BOOST_AUTO_TEST_CASE(remove_long_option_name)
{
    BOOST_CHECK_THROW(
        option({}, "--help").long_name(""),
	cli::error::option_must_have_at_least_short_or_long_name);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(representation);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(description);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_required);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(has_arguments);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(has_equality_validator);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(equality_validator);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_short_option_name);

BOOST_AUTO_TEST_CASE(invalid_short_name)
{
    BOOST_TEST(not cli::core::is_short_option_name(std::string_view {""}));
    BOOST_TEST(not cli::core::is_short_option_name(std::string_view {"-"}));
}

BOOST_AUTO_TEST_CASE(valid_short_name)
{
    BOOST_TEST(cli::core::is_short_option_name(std::string_view {"- "}));
    BOOST_TEST(cli::core::is_short_option_name(std::string_view {"--"}));
    BOOST_TEST(cli::core::is_short_option_name(std::string_view {"-h"}));
    BOOST_TEST(cli::core::is_short_option_name(std::string_view {"-1"}));
    BOOST_TEST(cli::core::is_short_option_name(std::string_view {"-#"}));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_long_option_name);

BOOST_AUTO_TEST_CASE(invalid_long_name)
{
    BOOST_TEST(not cli::core::is_long_option_name(std::string_view {""}));
    BOOST_TEST(not cli::core::is_long_option_name(std::string_view {"--"}));
}

BOOST_AUTO_TEST_CASE(valid_long_name)
{
    BOOST_TEST(cli::core::is_long_option_name(std::string_view {"-- "}));
    BOOST_TEST(cli::core::is_long_option_name(std::string_view {"---"}));
    BOOST_TEST(cli::core::is_long_option_name(std::string_view {"--h"}));
    BOOST_TEST(cli::core::is_long_option_name(std::string_view {"--="}));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_long_option_name_with_argument);

BOOST_AUTO_TEST_CASE(invalid_long_option_name_with_argument)
{
    BOOST_TEST(
        not cli::core::is_long_option_name_with_argument(std::string_view {"-- "}));
    BOOST_TEST(
        not cli::core::is_long_option_name_with_argument(std::string_view {"---"}));
    BOOST_TEST(
        not cli::core::is_long_option_name_with_argument(std::string_view {"--h"}));
}

BOOST_AUTO_TEST_CASE(valid_long_option_name_with_argument)
{
    BOOST_TEST(
        cli::core::is_long_option_name_with_argument(std::string_view {"--="}));
    BOOST_TEST(
        cli::core::is_long_option_name_with_argument(std::string_view {"-- ="}));
    BOOST_TEST(
        cli::core::is_long_option_name_with_argument(std::string_view {"--h="}));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_option_name);

BOOST_AUTO_TEST_CASE(valid_option_name)
{
    BOOST_TEST(cli::core::is_option_name(std::string_view {"-f"}));
    BOOST_TEST(cli::core::is_option_name(std::string_view {"--file"}));
    BOOST_TEST(cli::core::is_option_name(std::string_view {"--file="}));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(equality_operator);

BOOST_AUTO_TEST_CASE(equality)
{
    const option help {
	"-h",
	"--help"
    };

    BOOST_TEST((help == option {"-h", {"--help"}}));
}

BOOST_AUTO_TEST_CASE(inequality)
{
    const option help {
	"-h",
	"--help"
    };

    BOOST_TEST((help != option {"-h"}));
    BOOST_TEST((help != option {{}, "--help"}));
}

BOOST_AUTO_TEST_CASE(use_custom_validator)
{
    const option verbose {
	"-v",
	"--verbose",
	"-v, --[no-]verbose",
	"verbose mode",
	option::required::not_required,
	option::arguments::no_arguments,
	[](auto&& option_name)
	{
	    return (option_name == "-v"        ||
		    option_name == "--verbose" ||
		    option_name == "--no-verbose");
	}
    };

    BOOST_TEST((verbose == "-v"));
    BOOST_TEST((verbose == "--verbose"));
    BOOST_TEST((verbose == "--no-verbose"));
}

BOOST_AUTO_TEST_SUITE_END();
