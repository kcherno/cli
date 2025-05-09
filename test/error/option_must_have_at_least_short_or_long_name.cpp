#define BOOST_TEST_MODULE option_must_have_at_least_short_or_long_name

#include <boost/test/unit_test.hpp>

#include "error/option_must_have_at_least_short_or_long_name.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(default_constructor)
{
    BOOST_CHECK_EQUAL(
        option_must_have_at_least_short_or_long_name().what(),
	"option must have at least short or long name");
}

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    BOOST_CHECK_EQUAL(
        option_must_have_at_least_short_or_long_name("where").what(),
	"where: option must have at least short or long name");
}

BOOST_AUTO_TEST_SUITE_END();
