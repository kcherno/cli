#define BOOST_TEST_MODULE exception_source_information

#include <boost/test/unit_test.hpp>

#include "configuration/exception_source_information.hpp"

BOOST_AUTO_TEST_CASE(check_macro_contents)
{

#ifndef DISABLE_EXCEPTION_SOURCE_INFORMATION

    BOOST_CHECK_NE(EXCEPTION_SOURCE_INFORMATION, "");

#else

    BOOST_CHECK_EQUAL(EXCEPTION_SOURCE_INFORMATION, "");

#endif

}
