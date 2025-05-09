#define BOOST_TEST_MODULE invalid_format_for_long_option_name

#include <boost/test/unit_test.hpp>

#include "error/invalid_format_for_long_option_name.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    BOOST_CHECK_EQUAL(
        invalid_format_for_long_option_name("-help").what(),
	"-help invalid format for long option name");

    BOOST_CHECK_EQUAL(
        invalid_format_for_long_option_name("-help", "where").what(),
	"where: -help invalid format for long option name");
}

BOOST_AUTO_TEST_SUITE_END();
