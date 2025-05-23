#define BOOST_TEST_MODULE parser

#include <utility>

#include <boost/test/unit_test.hpp>

#include "core/dictionary.hpp"
#include "core/option.hpp"
#include "core/parser.hpp"

#include "error/option_is_required_but_not_added.hpp"
#include "error/option_already_added_as.hpp"
#include "error/option_expects_argument.hpp"
#include "error/unrecognized_option.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(default_initialization)
{
    BOOST_TEST(parser().empty());
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
    BOOST_TEST(not parser{dictionary {}}.empty());
}

BOOST_AUTO_TEST_CASE(move_initialization)
{
    parser parser_1 {
	dictionary {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    parser parser_2 {std::move(parser_1)};

    BOOST_TEST(parser_1.empty());

    BOOST_TEST(not parser_2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(add_dictionary);

BOOST_AUTO_TEST_CASE(add_dictionary)
{
    const dictionary general_options {
	option {
	    "-h",
	    "--help"
	}
    };

    parser parser;

    BOOST_TEST(not parser.contains(general_options));

    parser.add_dictionary(general_options);

    BOOST_TEST(parser.contains(general_options));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(contains);

BOOST_AUTO_TEST_CASE(check_not_added_option)
{
    const option verbose {
	"-v",
	"--verbose"
    };

    parser parser {
	dictionary {
	    verbose
	}
    };

    const char* argv[] = {
	"",
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv), argv));

    BOOST_TEST(not parser.contains(verbose));
}

BOOST_AUTO_TEST_CASE(check_added_option)
{
    const option verbose {
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
    };

    parser parser {
	dictionary {
	    verbose
	}
    };

    const char* argv_1[] = {
	"",
	"-v",
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv_1), argv_1));

    BOOST_TEST(parser.contains(verbose).has_value());

    const char* argv_2[] = {
	"",
	"--verbose",
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv_2), argv_2));

    BOOST_TEST(parser.contains(verbose).has_value());

    const char* argv_3[] = {
	"",
	"--no-verbose",
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv_3), argv_3));

    BOOST_TEST(parser.contains(verbose).has_value());
}

BOOST_AUTO_TEST_CASE(check_not_added_dictionary)
{
    BOOST_TEST(not parser().contains(dictionary {}));
}

BOOST_AUTO_TEST_CASE(check_added_dictionary)
{
    const dictionary general_options {
	option {
	    "-h",
	    "--help"
	}
    };

    BOOST_TEST(parser{general_options}.contains(general_options));
}

BOOST_AUTO_TEST_CASE(check_not_added_option_name)
{
    BOOST_TEST(not parser().contains("").has_value());
}

BOOST_AUTO_TEST_CASE(check_added_option_name)
{
    parser parser {
	dictionary {
	    option {
		"-v",
		"--verbose",
		"-v, --[no-]verbose",
		"verbose mode",
		option::required::not_required,
		option::arguments::no_arguments,
		[](auto&& option_name)
		{
		    return option_name == "-v" ||
			   option_name == "--verbose" ||
			   option_name == "--no-verbose";
		}
	    }
	}
    };

    const char* argv[] = {
	"",
	"-v",
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv), argv));

    BOOST_TEST(parser.contains("-v").has_value());
    BOOST_TEST(parser.contains("--verbose").has_value());
    BOOST_TEST(parser.contains("--no-verbose").has_value());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(erase);

BOOST_AUTO_TEST_CASE(erase_non_existent_dictionary)
{
    parser parser {
	dictionary {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    parser.erase(dictionary {});

    BOOST_TEST(not parser.empty());
}

BOOST_AUTO_TEST_CASE(erase_existing_dictionary)
{
    parser parser {
	dictionary {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    parser.erase(dictionary {option {"-h", "--help"}});

    BOOST_TEST(parser.empty());
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

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv), argv));

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
	cli::error::unrecognized_option
    );
}

BOOST_AUTO_TEST_CASE(parse_valid_short_option)
{
    parser parser {
	dictionary {
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

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv), argv));

    BOOST_REQUIRE_EQUAL(parser.options().size(), 1);

    BOOST_CHECK_EQUAL(parser.options()[0], "-h");
}

BOOST_AUTO_TEST_CASE(parse_valid_short_option_with_argument)
{
    parser parser {
	dictionary {
	    option {
		"-f",
		{},
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
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv), argv));

    BOOST_REQUIRE_EQUAL(parser.options().size(), 2);

    BOOST_CHECK_EQUAL(parser.options()[0], "-f");
    BOOST_CHECK_EQUAL(parser.options()[1], "a.txt");
}

BOOST_AUTO_TEST_CASE(parse_valid_short_option_with_missing_argument)
{
    parser parser {
	dictionary {
	    option {
		"-f",
		{},
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
	nullptr
    };

    BOOST_CHECK_THROW(parser.parse_command_line(std::size(argv), argv),
		      cli::error::option_expects_argument);
}

BOOST_AUTO_TEST_CASE(parse_valid_long_option)
{
    parser parser {
	dictionary {
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
	dictionary {
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

    const char* argv_1[] = {
	"",
	"--file",
	"a.txt",
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv_1), argv_1));

    BOOST_REQUIRE_EQUAL(parser.options().size(), 2);

    BOOST_CHECK_EQUAL(parser.options()[0], "--file");
    BOOST_CHECK_EQUAL(parser.options()[1], "a.txt");

    const char* argv_2[] = {
	"",
	"--file=a.txt",
	nullptr
    };

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv_2), argv_2));

    BOOST_REQUIRE_EQUAL(parser.options().size(), 1);

    BOOST_CHECK_EQUAL(parser.options()[0], "--file=a.txt");
}

BOOST_AUTO_TEST_CASE(parse_valid_long_option_with_missing_argument)
{
    parser parser {
	dictionary {
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

    const char* argv_1[] = {
	"",
	"--file",
	nullptr
    };

    BOOST_CHECK_THROW(parser.parse_command_line(std::size(argv_1), argv_1),
		      cli::error::option_expects_argument);

    const char* argv_2[] = {
	"",
	"--file=",
	nullptr
    };

    BOOST_CHECK_THROW(parser.parse_command_line(std::size(argv_2), argv_2),
		      cli::error::option_expects_argument);
}

BOOST_AUTO_TEST_CASE(parse_multiple_options_with_arguments)
{
    parser parser {
	dictionary {
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

    BOOST_CHECK_NO_THROW(parser.parse_command_line(std::size(argv), argv));

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
	dictionary {
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
	dictionary {
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
	cli::error::option_already_added_as
    );
}

BOOST_AUTO_TEST_CASE(parse_missing_required_option)
{
    parser parser {
	dictionary {
	    option {
		"-v",
		"--verbose",
		"-v, --[no-]verbose",
		"verbose mode",
		option::required::required
	    }
	}
    };

    const char* argv[] = {
	"",
	nullptr
    };

    BOOST_CHECK_THROW(parser.parse_command_line(std::size(argv), argv),
		      cli::error::option_is_required_but_not_added);
}

BOOST_AUTO_TEST_SUITE_END();
