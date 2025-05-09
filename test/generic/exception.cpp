#define BOOST_TEST_MODULE exception

#include <boost/test/unit_test.hpp>

#include "generic/exception.hpp"

using namespace cli::generic;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    BOOST_CHECK_EQUAL(exception("what").what(), "what");
    BOOST_CHECK_EQUAL(exception("what", "").what(), "what");
    BOOST_CHECK_EQUAL(exception("what", "where").what(), "where: what");
}

BOOST_AUTO_TEST_SUITE_END();
