#define BOOST_TEST_MODULE option_is_required_but_not_added

#include <boost/test/unit_test.hpp>

#include "error/option_is_required_but_not_added.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(initialize_argument_option)
{
    BOOST_CHECK_EQUAL(option_is_required_but_not_added("--verbose").what(),
		      "--verbose is required but not added");
}

BOOST_AUTO_TEST_CASE(full_initialization)
{
    BOOST_CHECK_EQUAL(option_is_required_but_not_added("--verbose", "where")
		          .what(),
		      "where: --verbose is required but not added");

}

BOOST_AUTO_TEST_SUITE_END();
