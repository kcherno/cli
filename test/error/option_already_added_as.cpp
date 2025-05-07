#define BOOST_TEST_MODULE option_already_added_as

#include <boost/test/unit_test.hpp>

#include "error/option_already_added_as.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(initialize_option_and_added_as)
{
    BOOST_CHECK_EQUAL(option_already_added_as("--verbose", "--no-verbose")
		          .what(),
		      "--verbose already added as --no-verbose");
}

BOOST_AUTO_TEST_CASE(full_initialization)
{
    BOOST_CHECK_EQUAL(option_already_added_as("--verbose", "--no-verbose", "where")
		          .what(),
		      "where: --verbose already added as --no-verbose");
}

BOOST_AUTO_TEST_SUITE_END();
