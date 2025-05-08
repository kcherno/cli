#include <string_view>
#include <algorithm>

#include "configuration/exception_source_information.hpp"

#include "core/option.hpp"
#include "core/parser.hpp"

#include "error/option_is_required_but_not_added.hpp"
#include "error/option_already_added_as.hpp"
#include "error/option_expects_argument.hpp"
#include "error/unrecognized_option.hpp"

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

    for (auto&& book : books)
    {
	for (auto&& option : book)
	{
	    if (option.is_required() && not contains(option))
	    {
		throw error::option_is_required_but_not_added {
		    option.short_name().empty() ?
			option.long_name() :
			option.short_name(),
		    EXCEPTION_SOURCE_INFORMATION
		};
	    }
	}
    }
}

void parser::add_option(std::string_view option_name)
{
    auto option = option_name.substr(0, option_name.find('='));

    if (book_contains(option))
    {
	if (not get_option_from_book(option).has_arguments())
	{
	    if (auto optional = contains(option); optional)
	    {
		throw error::option_already_added_as {
		    option,
			optional.value(),
		    EXCEPTION_SOURCE_INFORMATION
		};
	    }
	}

	options_.emplace_back(option_name);
    }

    else
    {
	throw error::unrecognized_option {
	    option,
	    EXCEPTION_SOURCE_INFORMATION
	};
    }
}

std::vector<std::string_view>::const_iterator
parser::find_option_with_validation(std::string_view option_name) const noexcept
{
    auto iterator = find_option_in_book(option_name);

    if (iterator != books.cend())
    {
	auto& option = iterator->operator[](option_name);

	return std::find_if(options_.cbegin(),
			    options_.cend(),
			    [&](auto&& op)
			    {
				return option == op;
			    });
    }

    return options_.cend();
}
