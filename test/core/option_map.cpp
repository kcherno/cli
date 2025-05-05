#define BOOST_TEST_MODULE option_map

#include <boost/test/unit_test.hpp>

#include "core/option_book.hpp"
#include "core/option_map.hpp"
#include "core/option.hpp"

using namespace cli::core;

BOOST_AUTO_TEST_SUITE(initialization);

BOOST_AUTO_TEST_CASE(default_initialization)
{
    const option_map option_map;

    BOOST_TEST(option_map.empty());
}

BOOST_AUTO_TEST_CASE(initializer_list)
{
    const option_map option_map {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    BOOST_TEST(option_map.contains(option_book {option {"-h", "--help"}}));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(add_command_line_options);

BOOST_AUTO_TEST_CASE(add_valid_command_line_options)
{
    const std::vector<std::string_view> command_line_options {
	"--verbose",
	"-f",
	"input.txt",
	"--output=result.json",
	"-n",
	"5",
	"--threads=8",
	"--config=settings.ini",
	"-r"
    };

    option_map option_map {
	option_book {
	    option {
		"-v",
		"--verbose"
	    },

	    option {
		"-f",
		"--file",
		"-f, --file",
		"add file",
		option::required::not_required,
		option::arguments::has_arguments
	    },

	    option {
		"-o",
		"--output",
		"-o, --output",
		"specify output file",
		option::required::not_required,
		option::arguments::has_arguments
	    },

	    option {
		"-n",
		"--number",
		"-n, --number",
		"specify number",
		option::required::not_required,
		option::arguments::has_arguments
	    },

	    option {
		"-t",
		"--threads",
		"-t, --threads",
		"specify threads",
		option::required::not_required,
		option::arguments::has_arguments
	    },

	    option {
		"-c",
		"--config",
		"-c, --config",
		"specify configuration files",
		option::required::not_required,
		option::arguments::has_arguments
	    },

	    option {
		"-r",
		"--recursive",
		"-r, --recursive",
		"recursive mode"
	    }
	}
    };

    BOOST_CHECK_NO_THROW(
        option_map.add_command_line_options(command_line_options);
    );

    BOOST_TEST(option_map.contains("--verbose"));

    BOOST_TEST(option_map["-f"] ==
	       option_map::mapped_type {"input.txt"});

    BOOST_TEST(option_map["--output"] ==
	       option_map::mapped_type {"result.json"});

    BOOST_TEST(option_map["-n"] ==
	       option_map::mapped_type {"5"});

    BOOST_TEST(option_map["--threads"] ==
	       option_map::mapped_type {"8"});

    BOOST_TEST(option_map["--config"] ==
	       option_map::mapped_type {"settings.ini"});

    BOOST_TEST(option_map.contains("-r"));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(add_option);

BOOST_AUTO_TEST_CASE(add_valid_short_option)
{
    option_map option_map {
	option_book {
	    option {
		"-h",
		"--help",
	    }
	}
    };

    option_map.add_option("-h");

    BOOST_TEST(option_map.contains("-h"));
}

BOOST_AUTO_TEST_CASE(add_valid_long_option)
{
    option_map option_map {
	option_book {
	    option {
		"-h",
		"--help",
	    }
	}
    };

    option_map.add_option("--help");

    BOOST_TEST(option_map.contains("--help"));
}

BOOST_AUTO_TEST_CASE(add_valid_long_option_with_argument)
{
    option_map option_map {
	option_book {
	    option {
		"-f",
		"--file",
		"-f, --file",
		"add file",
		option::required::not_required,
		option::arguments::has_arguments
	    }
	}
    };

    option_map.add_option("--file=a.txt,b.txt,c.txt");

    BOOST_TEST(option_map.contains("--file"));
    
    BOOST_TEST((option_map["--file"] ==
		option_map::mapped_type {"a.txt", "b.txt", "c.txt"}));
}

BOOST_AUTO_TEST_CASE(add_valid_long_option_without_argument)
{
    option_map option_map {
	option_book {
	    option {
		"-f",
		"--file",
		"-f, --file",
		"add file",
		option::required::not_required,
		option::arguments::has_arguments
	    }
	}
    };

    BOOST_CHECK_THROW(option_map.add_option("--file="), std::logic_error);
}

BOOST_AUTO_TEST_CASE(add_unrecognized_option)
{
    BOOST_CHECK_THROW(option_map().add_option("-h"), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(add_option_argument);

BOOST_AUTO_TEST_CASE(add_argument_to_option_with_arguments)
{
    option_map option_map {
	option_book {
	    option {
		"-f",
		"--file",
		"-f, --file",
		"add file",
		option::required::not_required,
		option::arguments::has_arguments
	    }
	}
    };

    option_map.add_option_argument("-f", "a.txt");

    BOOST_TEST((option_map["-f"]     == option_map::mapped_type {"a.txt"}));
    BOOST_TEST((option_map["--file"] == option_map::mapped_type {"a.txt"}));

    option_map.add_option_argument("--file", "b.txt");

    BOOST_TEST((option_map["-f"] ==
		option_map::mapped_type {"a.txt", "b.txt"}));

    BOOST_TEST((option_map["--file"] ==
		option_map::mapped_type {"a.txt", "b.txt"}));
}

BOOST_AUTO_TEST_CASE(add_empty_argument_to_option_with_arguments)
{
    option_map option_map {
	option_book {
	    option {
		"-f",
		"--file",
		"-f, --file",
		"add file",
		option::required::not_required,
		option::arguments::has_arguments
	    }
	}
    };

    BOOST_CHECK_THROW(option_map.add_option_argument("-f", {}),
		      std::logic_error);
}

BOOST_AUTO_TEST_CASE(add_argument_to_option_without_arguments)
{
    option_map option_map {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    BOOST_CHECK_THROW(option_map.add_option_argument("-h", "argument"),
		      std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(add_option_book);

BOOST_AUTO_TEST_CASE(assign_option_book)
{
    const option_book option_book {
	option {
	    "-v",
	    "--verbose"
	}
    };

    option_map option_map;

    BOOST_CHECK_EQUAL(option_map.contains(option_book), false);

    option_map.add_option_book(option_book);

    BOOST_TEST(option_map.contains(option_book));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(contains);

BOOST_AUTO_TEST_CASE(check_existing_option)
{
    option_map option_map {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    option_map.add_option("-h");

    BOOST_TEST(option_map.contains("-h"));
}

BOOST_AUTO_TEST_CASE(check_non_existent_option)
{
    option_map option_map {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    option_map.add_option("-h");

    BOOST_TEST(not option_map.contains("--help"));
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(bracket_operator);

BOOST_AUTO_TEST_CASE(access_to_arguments_of_unrecognized_option)
{
    BOOST_CHECK_THROW(option_map()["-h"], std::logic_error);
}

BOOST_AUTO_TEST_CASE(access_to_arguments_of_option_that_not_yet_added)
{
    const option_map option_map {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    BOOST_CHECK_THROW(option_map["-h"],     std::logic_error);
    BOOST_CHECK_THROW(option_map["--help"], std::logic_error);
}

BOOST_AUTO_TEST_CASE(access_to_arguments_of_option_that_has_no_arguments)
{
    option_map option_map {
	option_book {
	    option {
		"-h",
		"--help"
	    }
	}
    };

    option_map.add_option("-h");

    BOOST_CHECK_THROW(option_map["-h"], std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END();
