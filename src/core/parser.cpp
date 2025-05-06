#include <string_view>
#include <stdexcept>
#include <string>
#include <vector>

#include "core/option.hpp"
#include "core/parser.hpp"

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
		continue;
	    }

	    if (get_option_from_book(option_name).has_arguments())
	    {
		if (i + 1 < argc && not option::is_option_name(argv[i + 1]))
		{
		    add_option_argument(option_name, argv[i + 1]);
		}

		else
		{
		    throw std::logic_error {
			std::string(__func__)
			    .append(": ")
			    .append(option_name)
			    .append(" expects argument")
		    };
		}

		++i;
	    }

	    continue;
	}

	if (option::is_short_option_name(argv[i - 1]) ||
	    option::is_long_option_name(argv[i - 1]))
	{
	    if (get_option_from_book(argv[i - 1]).has_arguments())
	    {
		add_option_argument(argv[i - 1], argv[i]);

		continue;
	    }
	}

	positional_options_.emplace_back(argv[i]);
    }
}
