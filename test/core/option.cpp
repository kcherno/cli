#define BOOST_TEST_MODULE option

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

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    
}

BOOST_AUTO_TEST_CASE(move_constructor)
{

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

BOOST_AUTO_TEST_CASE(empty_representation)
{

}

BOOST_AUTO_TEST_CASE(not_empty_representation)
{

}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(description);

BOOST_AUTO_TEST_CASE(empty_description)
{

}

BOOST_AUTO_TEST_CASE(not_empty_description)
{

}

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
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_long_option_name);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_long_option_name_with_argument);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(is_option_name);
BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(equality_operator);
BOOST_AUTO_TEST_SUITE_END();
