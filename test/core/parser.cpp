#define BOOST_TEST_MODULE parser

#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "core/option_book.hpp"
#include "core/option.hpp"
#include "core/parser.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(add_option_book);

BOOST_AUTO_TEST_CASE(add_option_book)
{
    const option_book general_options {
	option {
	    "-h",
	    "--help"
	}
    };

    parser parser;

    BOOST_TEST(not parser.contains(general_options));

    parser.add_option_book(general_options);

    BOOST_TEST(parser.contains(general_options));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(clear);

BOOST_AUTO_TEST_CASE(clear_non_empty_parser)
{
    parser parser {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    BOOST_TEST(not parser.empty());

    parser.clear();

    BOOST_TEST(parser.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(contains);

BOOST_AUTO_TEST_CASE(check_non_existent_option_book)
{
    BOOST_TEST(not parser().contains(option_book {}));
}

BOOST_AUTO_TEST_CASE(check_existing_option_book)
{
    const option_book general_options {
	option {
	    "-h",
	    "--help"
	}
    };

    BOOST_TEST(parser{general_options}.contains(general_options));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(erase);

BOOST_AUTO_TEST_CASE(erase_non_existent_option_book)
{
    parser parser {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    parser.erase(option_book {});

    BOOST_TEST(not parser.empty());
}

BOOST_AUTO_TEST_CASE(erase_existing_option_book)
{
    parser parser {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    parser.erase(option_book {option {"-h", "--help"}});

    BOOST_TEST(parser.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(empty);

BOOST_AUTO_TEST_CASE(check_empty_parser)
{
    BOOST_TEST(parser().empty());
}

BOOST_AUTO_TEST_CASE(check_non_empty_parser)
{
    BOOST_TEST(not parser{option_book {}}.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(parse_command_line);

BOOST_AUTO_TEST_CASE(parse_empty_command_line)
{
    const char* argv[] = {
	"",
	nullptr
    };

    parser parser;

    parser.parse_command_line(std::size(argv), argv);

    BOOST_TEST(parser.options().empty());
    BOOST_TEST(parser.positional_options().empty());
}

BOOST_AUTO_TEST_CASE(parse_unrecognized_option)
{
    const char* argv[] = {
	"",
	"-h",
	nullptr
    };

    BOOST_CHECK_THROW(
        parser().parse_command_line(std::size(argv), argv),
	std::logic_error
    );
}

BOOST_AUTO_TEST_CASE(parse_valid_short_option)
{
    parser parser {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    const char* argv[] = {
	"",
	"-h",
	nullptr
    };

    parser.parse_command_line(std::size(argv), argv);

    BOOST_TEST(not parser.options().empty());

    BOOST_CHECK_EQUAL(parser.options().size(), 1);
    BOOST_CHECK_EQUAL(parser.options()[0],     "-h");
}

BOOST_AUTO_TEST_CASE(parse_valid_long_option)
{
    parser parser {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    const char* argv[] = {
	"",
	"--help",
	nullptr
    };

    parser.parse_command_line(std::size(argv), argv);

    BOOST_TEST(not parser.options().empty());

    BOOST_CHECK_EQUAL(parser.options().size(), 1);
    BOOST_CHECK_EQUAL(parser.options()[0],     "--help");
}

BOOST_AUTO_TEST_CASE(parse_valid_long_option_with_argument)
{
    parser parser {
	option_book {
	    option {
		{},
		"--file",
		{},
		{},
		option::required::not_required,
		option::arguments::has_arguments
	    }
	}
    };

    const char* argv[] = {
	"",
	"--file=a.txt",
	nullptr
    };

    parser.parse_command_line(std::size(argv), argv);

    BOOST_TEST(not parser.options().empty());

    BOOST_REQUIRE_EQUAL(parser.options().size(), 1);

    BOOST_CHECK_EQUAL(parser.options()[0], "--file=a.txt");
}

BOOST_AUTO_TEST_CASE(parse_multiple_options_with_arguments)
{
    parser parser {
	option_book {
	    option {
		"-f",
		"--file",
		{},
		{},
		option::required::not_required,
		option::arguments::has_arguments
	    }
	}
    };

    const char* argv[] = {
	"",
	"-f",
	"a.txt",
	"--file",
	"b.txt",
	"--file=c.txt",
	"-f",
	"d.txt",
	"--file",
	"e.txt",
	"--file=f.txt",
	nullptr
    };

    parser.parse_command_line(std::size(argv), argv);

    BOOST_REQUIRE_EQUAL(parser.options().size(), 10);

    BOOST_CHECK_EQUAL(parser.options()[0], "-f");
    BOOST_CHECK_EQUAL(parser.options()[1], "a.txt");
    BOOST_CHECK_EQUAL(parser.options()[2], "--file");
    BOOST_CHECK_EQUAL(parser.options()[3], "b.txt");
    BOOST_CHECK_EQUAL(parser.options()[4], "--file=c.txt");
    BOOST_CHECK_EQUAL(parser.options()[5], "-f");
    BOOST_CHECK_EQUAL(parser.options()[6], "d.txt");
    BOOST_CHECK_EQUAL(parser.options()[7], "--file");
    BOOST_CHECK_EQUAL(parser.options()[8], "e.txt");
    BOOST_CHECK_EQUAL(parser.options()[9], "--file=f.txt");
}

BOOST_AUTO_TEST_CASE(parse_options_and_positional_options)
{
    parser parser {
	option_book {
	    option {
		"-f",
		"--file",
		{},
		{},
		option::required::not_required,
		option::arguments::has_arguments
	    },

	    option {
		"-h",
		"--help"
	    },

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
	}
    };

    const char* argv[] = {
	"",
	"-f",
	"a.txt",
	"data.dat",
	"--file",
	"config.conf",
	"b.txt",
	"-h",
	"--file=c.txt",
	"-f",
	"d.txt",
	"--file",
	"e.txt",
	"--no-verbose",
	"output.out",
	"--file=f.txt",
	nullptr
    };

    parser.parse_command_line(std::size(argv), argv);

    BOOST_TEST(not parser.options().empty());

    BOOST_REQUIRE_EQUAL(parser.options().size(), 12);

    BOOST_CHECK_EQUAL(parser.options()[0],  "-f");
    BOOST_CHECK_EQUAL(parser.options()[1],  "a.txt");
    BOOST_CHECK_EQUAL(parser.options()[2],  "--file");
    BOOST_CHECK_EQUAL(parser.options()[3],  "config.conf");
    BOOST_CHECK_EQUAL(parser.options()[4],  "-h");
    BOOST_CHECK_EQUAL(parser.options()[5],  "--file=c.txt");
    BOOST_CHECK_EQUAL(parser.options()[6],  "-f");
    BOOST_CHECK_EQUAL(parser.options()[7],  "d.txt");
    BOOST_CHECK_EQUAL(parser.options()[8],  "--file");
    BOOST_CHECK_EQUAL(parser.options()[9],  "e.txt");
    BOOST_CHECK_EQUAL(parser.options()[10], "--no-verbose");
    BOOST_CHECK_EQUAL(parser.options()[11], "--file=f.txt");

    BOOST_TEST(not parser.positional_options().empty());

    BOOST_REQUIRE_EQUAL(parser.positional_options().size(), 3);

    BOOST_CHECK_EQUAL(parser.positional_options()[0], "data.dat");
    BOOST_CHECK_EQUAL(parser.positional_options()[1], "b.txt");
    BOOST_CHECK_EQUAL(parser.positional_options()[2], "output.out");
}

BOOST_AUTO_TEST_CASE(parse_already_added_option_without_argument)
{
    parser parser {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    const char* argv[] = {
	"",
	"-h",
	"--help",
	nullptr
    };

    BOOST_CHECK_THROW(
        parser.parse_command_line(std::size(argv), argv),
	std::logic_error
    );
}

BOOST_AUTO_TEST_SUITE_END();
