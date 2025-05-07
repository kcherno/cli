#define BOOST_TEST_MODULE unrecognized_option

#include <boost/test/unit_test.hpp>

#include "error/unrecognized_option.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(default_initialization)
{
    BOOST_CHECK_EQUAL(unrecognized_option().what(), "");
}

BOOST_AUTO_TEST_CASE(initialize_argument_option)
{
    BOOST_CHECK_EQUAL(unrecognized_option("-h").what(),
		      "unrecognized option -h");
}

BOOST_AUTO_TEST_CASE(full_initialization)
{
    BOOST_CHECK_EQUAL(unrecognized_option("-h", "where").what(),
		      "where: unrecognized option -h");
}

BOOST_AUTO_TEST_SUITE_END();
