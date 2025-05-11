#define BOOST_TEST_MODULE accessing_option_without_arguments

#include <boost/test/unit_test.hpp>

#include "error/accessing_option_without_arguments.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    BOOST_CHECK_EQUAL(
        accessing_option_without_arguments("option").what(),
	"accessing option without arguments");

    BOOST_CHECK_EQUAL(
        accessing_option_without_arguments("option", "where").what(),
	"where: accessing option without arguments");
}

BOOST_AUTO_TEST_SUITE_END();
