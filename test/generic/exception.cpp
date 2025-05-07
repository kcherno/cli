#define BOOST_TEST_MODULE exception

#include <boost/test/unit_test.hpp>

#include "generic/exception.hpp"

using namespace cli::generic;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(default_initialization)
{
    BOOST_CHECK_EQUAL(exception().what(), "");
}

BOOST_AUTO_TEST_CASE(initialize_argument_what)
{
    BOOST_CHECK_EQUAL(exception("what").what(), "what");
}

BOOST_AUTO_TEST_CASE(full_initialization)
{
    BOOST_CHECK_EQUAL(exception("what", "where").what(), "where: what");
}

BOOST_AUTO_TEST_SUITE_END();
