#define BOOST_TEST_MODULE option_map

#include <utility>

#include <boost/test/unit_test.hpp>

#include "core/dictionary.hpp"
#include "core/option_map.hpp"
#include "core/option.hpp"
#include "core/parser.hpp"

#include "error/accessing_option_without_arguments.hpp"
#include "error/accessing_option_not_yet_added.hpp"
#include "error/unrecognized_option.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(constructor);

BOOST_AUTO_TEST_CASE(default_constructor)
{
    BOOST_TEST(option_map().empty());
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
    BOOST_TEST(not option_map{dictionary {option {"-h"}}}.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(add_command_line_options);

BOOST_AUTO_TEST_CASE(add_mixed_command_line_options)
{
    const dictionary dictionary {
	option {
	    "-i",
	    "--interface",
	    "-i, --interface",
	    {},
	    option::required::required,
	    option::arguments::has_arguments
	},

	option {
	    "-o",
	    "--output",
	    "-o, --output",
	    {},
	    option::required::not_required,
	    option::arguments::has_arguments
	},

	option {
	    "-v",
	    "--verbose",
	    "-v, --[no-]verbose",
	    {},
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

    const char* argv[] = {
	"",
	"--no-verbose",
	"-i",
	"lo",
	"--interface",
	"pan0",
	"--interface=,wlan0,vmnet1,,,,ppp0,,,",
	"--output=a.dat,b.dat,,,,c.dat",
	"-o",
	"d.dat",
	"--output",
	"e.dat",
	"-i",
	"eth0",
	"--interface=vmnet8,vboxnet0",
	nullptr
    };

    parser parser {dictionary};

    parser.parse_command_line(std::size(argv), argv);

    option_map map {dictionary};

    map.add_command_line_options(parser.options());

    BOOST_TEST(map.contains("-i").has_value());
    BOOST_TEST(map.contains("--interface").has_value());

    BOOST_REQUIRE_EQUAL(map["-i"].size(),          8);
    BOOST_REQUIRE_EQUAL(map["--interface"].size(), 8);

    BOOST_CHECK_EQUAL(map["-i"][0], "lo");
    BOOST_CHECK_EQUAL(map["-i"][1], "pan0");
    BOOST_CHECK_EQUAL(map["-i"][2], "wlan0");
    BOOST_CHECK_EQUAL(map["-i"][3], "vmnet1");
    BOOST_CHECK_EQUAL(map["-i"][4], "ppp0");
    BOOST_CHECK_EQUAL(map["-i"][5], "eth0");
    BOOST_CHECK_EQUAL(map["-i"][6], "vmnet8");
    BOOST_CHECK_EQUAL(map["-i"][7], "vboxnet0");

    BOOST_CHECK_EQUAL(map["--interface"][0], "lo");
    BOOST_CHECK_EQUAL(map["--interface"][1], "pan0");
    BOOST_CHECK_EQUAL(map["--interface"][2], "wlan0");
    BOOST_CHECK_EQUAL(map["--interface"][3], "vmnet1");
    BOOST_CHECK_EQUAL(map["--interface"][4], "ppp0");
    BOOST_CHECK_EQUAL(map["--interface"][5], "eth0");
    BOOST_CHECK_EQUAL(map["--interface"][6], "vmnet8");
    BOOST_CHECK_EQUAL(map["--interface"][7], "vboxnet0");

    BOOST_TEST(map.contains("-o").has_value());
    BOOST_TEST(map.contains("--output").has_value());

    BOOST_REQUIRE_EQUAL(map["-o"].size(),       5);
    BOOST_REQUIRE_EQUAL(map["--output"].size(), 5);

    BOOST_CHECK_EQUAL(map["-o"][0], "a.dat");
    BOOST_CHECK_EQUAL(map["-o"][1], "b.dat");
    BOOST_CHECK_EQUAL(map["-o"][2], "c.dat");
    BOOST_CHECK_EQUAL(map["-o"][3], "d.dat");
    BOOST_CHECK_EQUAL(map["-o"][4], "e.dat");

    BOOST_CHECK_EQUAL(map["--output"][0], "a.dat");
    BOOST_CHECK_EQUAL(map["--output"][1], "b.dat");
    BOOST_CHECK_EQUAL(map["--output"][2], "c.dat");
    BOOST_CHECK_EQUAL(map["--output"][3], "d.dat");
    BOOST_CHECK_EQUAL(map["--output"][4], "e.dat");

    BOOST_TEST(map.contains("-v").has_value());
    BOOST_TEST(map.contains("--verbose").has_value());
    BOOST_TEST(map.contains("--no-verbose").has_value());

    BOOST_CHECK_EQUAL(map.contains("-v").value(),           "--no-verbose");
    BOOST_CHECK_EQUAL(map.contains("--verbose").value(),    "--no-verbose");
    BOOST_CHECK_EQUAL(map.contains("--no-verbose").value(), "--no-verbose");
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(add_dictionary);

BOOST_AUTO_TEST_SUITE(copy_dictionary);

BOOST_AUTO_TEST_CASE(empty_dictionary)
{
    const dictionary dictionary;

    option_map map;

    map.add_dictionary(dictionary);

    BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(non_empty_dictionary)
{
    const dictionary dictionary {
	option {
	    "-h",
	    "--help"
	}
    };

    option_map map;

    map.add_dictionary(dictionary);

    BOOST_TEST(not map.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(move_dictionary);

BOOST_AUTO_TEST_CASE(empty_dictionary)
{
    option_map map;

    map.add_dictionary(dictionary {});

    BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(non_empty_dictionary)
{
    dictionary dictionary {
	option {
	    "-h",
	    "--help"
	}
    };

    option_map map;

    map.add_dictionary(std::move(dictionary));

    BOOST_TEST(dictionary.empty());

    BOOST_TEST(not map.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(contains);

BOOST_AUTO_TEST_CASE(check_added_dictionary)
{
    const dictionary dictionary {
	option {
	    "-h",
	    "--help"
	}
    };

    option_map map;

    map.add_dictionary(dictionary);

    BOOST_TEST(map.contains(dictionary));
}

BOOST_AUTO_TEST_CASE(check_unadded_dictionary)
{
    const dictionary dictionary {
	option {
	    "-h",
	    "--help"
	}
    };

    option_map map;

    BOOST_TEST(not map.contains(dictionary));
}

BOOST_AUTO_TEST_CASE(check_added_option)
{
    const option help {"-h", "--help"};

    const dictionary dictionary {help};

    const char* argv[] = {
	"",
	"-h",
	nullptr
    };

    parser parser {dictionary};

    parser.parse_command_line(std::size(argv), argv);

    option_map map {dictionary};

    map.add_command_line_options(parser.options());

    BOOST_TEST(map.contains(help));
}

BOOST_AUTO_TEST_CASE(check_unadded_option)
{
    const option help {"-h", "--help"};

    const dictionary dictionary {help};

    const char* argv[] = {
	"",
	nullptr
    };

    parser parser {dictionary};

    parser.parse_command_line(std::size(argv), argv);

    option_map map {dictionary};

    map.add_command_line_options(parser.options());

    BOOST_TEST(not map.contains(help));
}

BOOST_AUTO_TEST_CASE(check_added_option_name)
{
    const dictionary dictionary {
	option {
	    "-v",
	    "--verbose",
	    "-v, --[no-]verbose",
	    {},
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

    const char* argv[] = {
	"",
	"-v",
	nullptr
    };

    parser parser {dictionary};

    parser.parse_command_line(std::size(argv), argv);

    option_map map {dictionary};

    map.add_command_line_options(parser.options());

    BOOST_TEST(map.contains("-v").has_value());
    BOOST_TEST(map.contains("--verbose").has_value());
    BOOST_TEST(map.contains("--no-verbose").has_value());
}

BOOST_AUTO_TEST_CASE(check_unadded_option_name)
{
    const dictionary dictionary {
	option {
	    "-v",
	    "--verbose",
	    "-v, --[no-]verbose",
	    {},
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

    const char* argv[] = {
	"",
	nullptr
    };

    parser parser {dictionary};

    parser.parse_command_line(std::size(argv), argv);

    option_map map {dictionary};

    map.add_command_line_options(parser.options());

    BOOST_TEST(not map.contains("-v").has_value());
    BOOST_TEST(not map.contains("--verbose").has_value());
    BOOST_TEST(not map.contains("--no-verbose").has_value());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(bracket_operator);

BOOST_AUTO_TEST_CASE(access_to_added_option_with_arguments)
{
    const option file {
	"-f",
	"--file",
	{},
	{},
	option::required::not_required,
	option::arguments::has_arguments
    };

    const dictionary dictionary {file};

    const char* argv_1[] = {
	"",
	"-f",
	"a.txt",
	nullptr
    };

    parser parser {dictionary};

    parser.parse_command_line(std::size(argv_1), argv_1);

    option_map map {dictionary};

    map.add_command_line_options(parser.options());

    BOOST_CHECK_NO_THROW(map[file]);
    BOOST_CHECK_NO_THROW(map["-f"]);
    BOOST_CHECK_NO_THROW(map["--file"]);

    BOOST_REQUIRE_EQUAL(map[file].size(),     1);
    BOOST_REQUIRE_EQUAL(map["-f"].size(),     1);
    BOOST_REQUIRE_EQUAL(map["--file"].size(), 1);

    BOOST_CHECK_EQUAL(map[file][0],     "a.txt");
    BOOST_CHECK_EQUAL(map["-f"][0],     "a.txt");
    BOOST_CHECK_EQUAL(map["--file"][0], "a.txt");

    const char* argv_2 [] = {
	"",
	"--file",
	"b.txt",
	nullptr
    };

    parser.parse_command_line(std::size(argv_2), argv_2);

    map.add_command_line_options(parser.options());

    BOOST_REQUIRE_EQUAL(map[file].size(),     2);
    BOOST_REQUIRE_EQUAL(map["-f"].size(),     2);
    BOOST_REQUIRE_EQUAL(map["--file"].size(), 2);

    BOOST_CHECK_EQUAL(map[file][1],     "b.txt");
    BOOST_CHECK_EQUAL(map["-f"][1],     "b.txt");
    BOOST_CHECK_EQUAL(map["--file"][1], "b.txt");

    const char* argv_3 [] = {
	"",
	"--file=c.txt",
	nullptr
    };

    parser.parse_command_line(std::size(argv_3), argv_3);

    map.add_command_line_options(parser.options());

    BOOST_REQUIRE_EQUAL(map[file].size(),     3);
    BOOST_REQUIRE_EQUAL(map["-f"].size(),     3);
    BOOST_REQUIRE_EQUAL(map["--file"].size(), 3);

    BOOST_CHECK_EQUAL(map[file][2],     "c.txt");
    BOOST_CHECK_EQUAL(map["-f"][2],     "c.txt");
    BOOST_CHECK_EQUAL(map["--file"][2], "c.txt");

    const char* argv_4 [] = {
	"",
	"--file=,d.txt,,e.txt,,,f.txt,",
	nullptr
    };

    parser.parse_command_line(std::size(argv_4), argv_4);

    map.add_command_line_options(parser.options());

    BOOST_REQUIRE_EQUAL(map[file].size(),     6);
    BOOST_REQUIRE_EQUAL(map["-f"].size(),     6);
    BOOST_REQUIRE_EQUAL(map["--file"].size(), 6);

    BOOST_CHECK_EQUAL(map[file][3],     "d.txt");
    BOOST_CHECK_EQUAL(map["-f"][3],     "d.txt");
    BOOST_CHECK_EQUAL(map["--file"][3], "d.txt");

    BOOST_CHECK_EQUAL(map[file][4],     "e.txt");
    BOOST_CHECK_EQUAL(map["-f"][4],     "e.txt");
    BOOST_CHECK_EQUAL(map["--file"][4], "e.txt");

    BOOST_CHECK_EQUAL(map[file][5],     "f.txt");
    BOOST_CHECK_EQUAL(map["-f"][5],     "f.txt");
    BOOST_CHECK_EQUAL(map["--file"][5], "f.txt");
}

BOOST_AUTO_TEST_CASE(access_to_unadded_option_with_arguments)
{
    const option file {
	"-f",
	"--file",
	{},
	{},
	option::required::not_required,
	option::arguments::has_arguments
    };

    option_map map {dictionary {file}};

    BOOST_CHECK_THROW(map[file], cli::error::accessing_option_not_yet_added);
    BOOST_CHECK_THROW(map["-f"], cli::error::accessing_option_not_yet_added);
    BOOST_CHECK_THROW(
        map["--file"], cli::error::accessing_option_not_yet_added);
}

BOOST_AUTO_TEST_CASE(access_to_added_option_without_arguments)
{
    const option help {
	"-h",
	"--help"
    };

    const dictionary dictionary {help};

    parser parser {dictionary};

    const char* argv[] = {
	"",
	"-h",
	nullptr
    };

    parser.parse_command_line(std::size(argv), argv);

    option_map map {dictionary};

    map.add_command_line_options(parser.options());

    BOOST_CHECK_THROW(
        map[help], cli::error::accessing_option_without_arguments);

    BOOST_CHECK_THROW(
        map["-h"], cli::error::accessing_option_without_arguments);

    BOOST_CHECK_THROW(
        map["--help"], cli::error::accessing_option_without_arguments);
}

BOOST_AUTO_TEST_CASE(access_to_unadded_option_without_arguments)
{
    const option help {
	"-h",
	"--help"
    };

    const option_map map {dictionary {help}};

    BOOST_CHECK_THROW(map[help], cli::error::accessing_option_not_yet_added);
    BOOST_CHECK_THROW(map["-h"], cli::error::accessing_option_not_yet_added);
    BOOST_CHECK_THROW(
        map["--help"], cli::error::accessing_option_not_yet_added);
}

BOOST_AUTO_TEST_CASE(access_to_unrecognized_option)
{
    const option help {
	"-h",
	"--help"
    };

    const option_map map;

    BOOST_CHECK_THROW(map[help], cli::error::unrecognized_option);
    BOOST_CHECK_THROW(map["-h"], cli::error::unrecognized_option);
    BOOST_CHECK_THROW(map["--help"], cli::error::unrecognized_option);
}

BOOST_AUTO_TEST_SUITE_END();
