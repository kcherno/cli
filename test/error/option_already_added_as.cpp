#define BOOST_TEST_MODULE option_already_added_as

#include <boost/test/unit_test.hpp>

#include "error/option_already_added_as.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    BOOST_CHECK_EQUAL(
        option_already_added_as("--verbose", "--no-verbose").what(),
	"--verbose already added as --no-verbose");

    BOOST_CHECK_EQUAL(
        option_already_added_as("--verbose", "--no-verbose", "where").what(),
	"where: --verbose already added as --no-verbose");
}

BOOST_AUTO_TEST_SUITE_END();
