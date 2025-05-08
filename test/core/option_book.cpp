#define BOOST_TEST_MODULE option_book

#include <stdexcept>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "core/option_book.hpp"
#include "core/option.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(default_initialization)
{
    BOOST_TEST(option_book().empty());
}

BOOST_AUTO_TEST_CASE(move_initialization)
{
    option_book option_book_1 {
	option {
	    "-h",
	    "--help"
	}
    };

    BOOST_TEST(not option_book_1.empty());

    option_book option_book_2 {std::move(option_book_1)};

    BOOST_TEST(option_book_1.empty());

    BOOST_TEST(not option_book_2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(contains);

BOOST_AUTO_TEST_CASE(check_existing_option)
{
    const option_book option_book {
	option {"-h", "--help"}
    };

    BOOST_TEST(option_book.contains("-h"));
    BOOST_TEST(option_book.contains("--help"));
    BOOST_TEST(option_book.contains(option {"-h", "--help"}));
}

BOOST_AUTO_TEST_CASE(check_non_existent_option)
{
    BOOST_TEST(not option_book().contains(option {}));
    BOOST_TEST(not option_book().contains(""));
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
		return option_name == "-v"        ||
		       option_name == "--verbose" ||
		       option_name == "--no-verbose";
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
    BOOST_CHECK_NO_THROW(option_book["--help"]);
}

BOOST_AUTO_TEST_CASE(access_to_non_existent_option)
{
    BOOST_CHECK_THROW(option_book()[""],        std::out_of_range);
    BOOST_CHECK_THROW(option_book()[option {}], std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END();
