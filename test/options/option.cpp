#define BOOST_TEST_MODULE option

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "options/option.hpp"

BOOST_AUTO_TEST_SUITE(option);

BOOST_AUTO_TEST_SUITE(representation);

BOOST_AUTO_TEST_CASE(no_names)
{
    BOOST_TEST(options::option().representation().empty());
}

BOOST_AUTO_TEST_CASE(only_short_name)
{
    const options::option help {
        .short_name    = "-h",
        .long_name     = {},
        .description   = {},
        .is_required   = {},
        .has_arguments = {}
    };

    BOOST_CHECK_EQUAL(help.representation(), "-h");
}

BOOST_AUTO_TEST_CASE(only_long_name)
{
    const options::option help {
        .short_name    = {},
        .long_name     = "--help",
        .description   = {},
        .is_required   = {},
        .has_arguments = {}
    };

    BOOST_CHECK_EQUAL(help.representation(), "--help");
}

BOOST_AUTO_TEST_CASE(both_names)
{
    const options::option help {
        .short_name    = "-h",
        .long_name     = "--help",
        .description   = {},
        .is_required   = {},
        .has_arguments = {}
    };

    BOOST_CHECK_EQUAL(help.representation(), "-h, --help");
}

BOOST_AUTO_TEST_SUITE_END(); // option/representation

BOOST_AUTO_TEST_SUITE_END(); // option
