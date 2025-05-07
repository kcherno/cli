#include <string_view>

#include "configuration/exception_source_information.hpp"

#include "core/option.hpp"
#include "core/parser.hpp"

#include "error/option_expects_argument.hpp"

using namespace cli::core;

void parser::parse_command_line(int argc, const char** argv)
{
    options_.clear();

    positional_options_.clear();

    for (int i = 1; i < argc && argv[i]; ++i)
    {
	std::string_view option_name = argv[i];

	if (option::is_option_name(option_name))
	{
	    add_option(option_name);

	    if (option::is_long_option_name_with_argument(option_name))
	    {
		if (option_name.find('=') == option_name.size() - 1)
		{
		    throw error::option_expects_argument {
			option_name,
		        EXCEPTION_SOURCE_INFORMATION
		    };
		}

		continue;
	    }

	    if (get_option_from_book(option_name).has_arguments())
	    {
		if (i + 1 < argc &&
		    argv[i + 1]  &&
		    not option::is_option_name(argv[i + 1]))
		{
		    options_.emplace_back(argv[++i]);
		}

		else
		{
		    throw error::option_expects_argument {
			option_name,
		        EXCEPTION_SOURCE_INFORMATION
		    };
		}
	    }

	    continue;
	}

	option_name = argv[i - 1];

	if (option::is_short_option_name(option_name) ||
	    option::is_long_option_name(option_name))
	{
	    if (get_option_from_book(option_name).has_arguments())
	    {
		options_.emplace_back(argv[i]);

		continue;
	    }
	}

	positional_options_.emplace_back(argv[i]);
    }

    // TODO(#1): add check for required options
}
