#define BOOST_TEST_MODULE option_expects_argument

#include <boost/test/unit_test.hpp>

#include "error/option_expects_argument.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(initilize_argument_option)
{
    BOOST_CHECK_EQUAL(option_expects_argument("--file=").what(),
		      "--file= expects argument");
}

BOOST_AUTO_TEST_CASE(full_initialization)
{
    BOOST_CHECK_EQUAL(option_expects_argument("--file=", "where").what(),
		      "where: --file= expects argument");
}

BOOST_AUTO_TEST_SUITE_END();
