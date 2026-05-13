#define BOOST_TEST_MODULE verification

#define BOOST_TEST_DYN_LINK

#include "boost/test/unit_test.hpp"

#include "detail/verification.hpp"

BOOST_AUTO_TEST_SUITE(verification);

BOOST_AUTO_TEST_SUITE(is_short_option_name);

BOOST_AUTO_TEST_SUITE(invalid_short_option_name);

BOOST_AUTO_TEST_CASE(empty)
{
    BOOST_TEST(not cli::detail::is_short_option_name(""));
}

BOOST_AUTO_TEST_CASE(size_less_than_2)
{
    BOOST_TEST(not cli::detail::is_short_option_name("-"));
}

BOOST_AUTO_TEST_CASE(does_not_start_with_a_dash)
{
    BOOST_TEST(not cli::detail::is_short_option_name("az"));
}

BOOST_AUTO_TEST_CASE(contains_a_punctuation_character)
{
    BOOST_TEST(not cli::detail::is_short_option_name("-!"));
}

BOOST_AUTO_TEST_SUITE_END();
// verification/is_short_option_name/invalid_short_option_name

BOOST_AUTO_TEST_SUITE(valid_short_option_name);

BOOST_AUTO_TEST_CASE(ends_with_a_lowercase_character)
{
    BOOST_TEST(cli::detail::is_short_option_name("-a"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_uppercase_character)
{
    BOOST_TEST(cli::detail::is_short_option_name("-A"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_digit)
{
    BOOST_TEST(cli::detail::is_short_option_name("-0"));
}

BOOST_AUTO_TEST_SUITE_END();
// verification/is_short_option_name/valid_short_option_name

BOOST_AUTO_TEST_SUITE_END(); // verification/is_short_option_name

BOOST_AUTO_TEST_SUITE(is_long_option_name);

BOOST_AUTO_TEST_SUITE(invalid_long_option_name);

BOOST_AUTO_TEST_CASE(empty)
{
    BOOST_TEST(not cli::detail::is_long_option_name(""));
}

BOOST_AUTO_TEST_CASE(size_less_than_3)
{
    BOOST_TEST(not cli::detail::is_long_option_name("abc"));
}

BOOST_AUTO_TEST_CASE(does_not_start_with_two_dashes)
{
    BOOST_TEST(not cli::detail::is_long_option_name("abcd"));
}

BOOST_AUTO_TEST_CASE(contains_a_space_character)
{
    BOOST_TEST(not cli::detail::is_long_option_name("--a "));
}

BOOST_AUTO_TEST_CASE(contains_a_punctuation_character)
{
    BOOST_TEST(not cli::detail::is_long_option_name("--a!"));
}

BOOST_AUTO_TEST_CASE(contains_two_dashes)
{
    BOOST_TEST(not cli::detail::is_long_option_name("--a--"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_punctuation_character)
{
    BOOST_TEST(not cli::detail::is_long_option_name("--a-"));
}

BOOST_AUTO_TEST_SUITE_END();
// verification/is_long_option_name/invalid_long_option_name

BOOST_AUTO_TEST_SUITE(valid_long_option_name);

BOOST_AUTO_TEST_CASE(ends_with_a_lowercase_character)
{
    BOOST_TEST(cli::detail::is_long_option_name("--Aa"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_uppercase_character)
{
    BOOST_TEST(cli::detail::is_long_option_name("--aA"));
}

BOOST_AUTO_TEST_CASE(ends_with_a_digit)
{
    BOOST_TEST(cli::detail::is_long_option_name("--a0"));
}

BOOST_AUTO_TEST_CASE(contains_dashes)
{
    BOOST_TEST(cli::detail::is_long_option_name("--a-b-c-d"));
}

BOOST_AUTO_TEST_SUITE_END();
// verification/is_long_option_name/valid_long_option_name

BOOST_AUTO_TEST_SUITE_END(); // verification/is_long_option_name

BOOST_AUTO_TEST_SUITE_END(); // verification
