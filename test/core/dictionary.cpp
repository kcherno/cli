#define BOOST_TEST_MODULE dictionary

#include <stdexcept>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "core/dictionary.hpp"
#include "core/option.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(default_constructor)
{
    BOOST_TEST(dictionary().empty());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    dictionary dictionary_1 {
	option {
	    "-h",
	    "--help"
	}
    };

    BOOST_TEST(not dictionary_1.empty());

    dictionary dictionary_2 {std::move(dictionary_1)};

    BOOST_TEST(dictionary_1.empty());

    BOOST_TEST(not dictionary_2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(contains);

BOOST_AUTO_TEST_CASE(check_existing_option)
{
    const dictionary dictionary {
	option {"-h", "--help"}
    };

    BOOST_TEST(dictionary.contains("-h"));
    BOOST_TEST(dictionary.contains("--help"));
    BOOST_TEST(dictionary.contains(option {"-h", "--help"}));
}

BOOST_AUTO_TEST_CASE(check_non_existent_option)
{
    BOOST_TEST(not dictionary().contains(""));
    BOOST_TEST(not dictionary().contains(option {"-h"}));
}

BOOST_AUTO_TEST_CASE(check_existing_option_using_custom_validator)
{
    const dictionary dictionary {
	option {
	    "-v",
	    "--verbose",
	    "-v, --[no-]verbose",
	    "verbose mode",
	    option::required::not_required,
	    option::arguments::no_arguments,
	    [](auto&& option_name)
	    {
		return (option_name == "-v"        ||
			option_name == "--verbose" ||
			option_name == "--no-verbose");
	    }
	}
    };

    BOOST_TEST(dictionary.contains("-v"));
    BOOST_TEST(dictionary.contains("--verbose"));
    BOOST_TEST(dictionary.contains("--no-verbose"));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(bracket_operator);

BOOST_AUTO_TEST_CASE(access_to_existing_option)
{
    const dictionary dictionary {
	option {"-h", "--help"}
    };

    BOOST_CHECK_NO_THROW(dictionary["-h"]);
    BOOST_CHECK_NO_THROW(dictionary["--help"]);
}

BOOST_AUTO_TEST_CASE(access_to_non_existent_option)
{
    BOOST_CHECK_THROW(dictionary()[""],            std::out_of_range);
    BOOST_CHECK_THROW(dictionary()[option {"-h"}], std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END();
