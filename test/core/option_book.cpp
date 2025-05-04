#define BOOST_TEST_MODULE option_book

#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "core/option_book.hpp"
#include "core/option.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(contains);

BOOST_AUTO_TEST_CASE(check_existing_option)
{
    const option_book option_book {
	option {"-h", "--help"}
    };

    BOOST_TEST(option_book.contains("-h"));
    BOOST_TEST(option_book.contains("--help"));
}

BOOST_AUTO_TEST_CASE(check_non_existent_option)
{
    BOOST_CHECK_EQUAL(option_book().contains("-h"), false);
}

BOOST_AUTO_TEST_CASE(check_existing_option_using_custom_validator)
{
    const option_book option_book {
	option {
	    "-v",
	    "--verbose",
	    "-v, --[no-]verbose",
	    "verbose mode",
	    option::required::not_required,
	    option::arguments::no_arguments,
	    [](auto&& option_name)
	    {
		if (option_name == "-v")
		{
		    return true;
		}

		if (option_name == "--verbose")
		{
		    return true;
		}

		if (option_name == "--no-verbose")
		{
		    return true;
		}

		return false;
	    }
	}
    };

    BOOST_TEST(option_book.contains("-v"));
    BOOST_TEST(option_book.contains("--verbose"));
    BOOST_TEST(option_book.contains("--no-verbose"));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(bracket_operator);

BOOST_AUTO_TEST_CASE(access_to_existing_option)
{
    const option_book option_book {
	option {"-h", "--help"}
    };

    BOOST_CHECK_NO_THROW(option_book["-h"]);
}

BOOST_AUTO_TEST_CASE(access_to_non_existent_option)
{
    BOOST_CHECK_THROW(option_book()["-h"], std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END();
