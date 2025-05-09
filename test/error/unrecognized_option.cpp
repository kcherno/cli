#define BOOST_TEST_MODULE unrecognized_option

#include <boost/test/unit_test.hpp>

#include "error/unrecognized_option.hpp"

using namespace cli::error;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(parameterized_constructor)
{
    BOOST_CHECK_EQUAL(
        unrecognized_option("-h").what(), "unrecognized option -h");

    BOOST_CHECK_EQUAL(
        unrecognized_option("-h", "where").what(),
	"where: unrecognized option -h");
}

BOOST_AUTO_TEST_SUITE_END();
