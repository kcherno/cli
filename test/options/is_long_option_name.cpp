#define BOOST_TEST_MODULE is_long_option_name

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "options/is_long_option_name.hpp"

using options::is_long_option_name;

BOOST_AUTO_TEST_SUITE(is_invalid_long_name);

BOOST_AUTO_TEST_CASE(empty)
{
    BOOST_TEST(not is_long_option_name(""));
}

BOOST_AUTO_TEST_CASE(size_less_than_3)
{
    BOOST_TEST(not is_long_option_name("abc"));
}

BOOST_AUTO_TEST_CASE(does_not_start_with_two_dashes)
{
    BOOST_TEST(not is_long_option_name("abcd"));
}

BOOST_AUTO_TEST_CASE(the_first_character_is_not_a_alphabetic)
{
    BOOST_TEST(not is_long_option_name("--0a"));
}

BOOST_AUTO_TEST_CASE(contains_a_space_character)
{
    BOOST_TEST(not is_long_option_name("--a "));
}

BOOST_AUTO_TEST_CASE(contains_a_punctuation_character)
{
    BOOST_TEST(not is_long_option_name("--a!"));
}

BOOST_AUTO_TEST_CASE(contains_two_dashes)
{
    BOOST_TEST(not is_long_option_name("--a--"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_punctuation_character)
{
    BOOST_TEST(not is_long_option_name("--a-"));
}

BOOST_AUTO_TEST_SUITE_END(); // is_invalid_long_name

BOOST_AUTO_TEST_SUITE(valid_long_option_name);

BOOST_AUTO_TEST_CASE(ends_with_a_lowercase_character)
{
    BOOST_TEST(is_long_option_name("--Aa"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_uppercase_character)
{
    BOOST_TEST(is_long_option_name("--aA"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_digit)
{
    BOOST_TEST(is_long_option_name("--a0"));
}

BOOST_AUTO_TEST_CASE(contains_dashes)
{
    BOOST_TEST(is_long_option_name("--a-b-c-d"));
}

BOOST_AUTO_TEST_SUITE_END(); // valid_long_option_name
