#define BOOST_TEST_MODULE is_short_option_name

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "options/is_short_option_name.hpp"

BOOST_AUTO_TEST_SUITE(invalid_short_option_name);

BOOST_AUTO_TEST_CASE(empty)
{
    BOOST_TEST(not options::is_short_option_name(""));
}

BOOST_AUTO_TEST_CASE(size_less_than_2)
{
    BOOST_TEST(not options::is_short_option_name("-"));
}

BOOST_AUTO_TEST_CASE(does_not_start_with_a_dash)
{
    BOOST_TEST(not options::is_short_option_name("az"));
}

BOOST_AUTO_TEST_CASE(contains_a_punctuation_character)
{
    BOOST_TEST(not options::is_short_option_name("-!"));
}

BOOST_AUTO_TEST_SUITE_END(); // invalid_short_option_name

BOOST_AUTO_TEST_SUITE(valid_short_option_name);

BOOST_AUTO_TEST_CASE(ends_with_a_lowercase_character)
{
    BOOST_TEST(options::is_short_option_name("-a"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_uppercase_character)
{
    BOOST_TEST(options::is_short_option_name("-A"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_digit)
{
    BOOST_TEST(options::is_short_option_name("-0"));
}

BOOST_AUTO_TEST_SUITE_END(); // valid_short_option_name
