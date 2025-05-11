#define BOOST_TEST_MODULE accessing_optin_not_yet_added

#include <boost/test/unit_test.hpp>

#include "error/accessing_option_not_yet_added.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    BOOST_CHECK_EQUAL(
        accessing_option_not_yet_added("option").what(),
	"accessing option not yet added");

    BOOST_CHECK_EQUAL(
        accessing_option_not_yet_added("option", "where").what(),
	"where: accessing option not yet added");
}

BOOST_AUTO_TEST_SUITE_END();
