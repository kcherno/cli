#include <string_view>
#include <stdexcept>
#include <string>
#include <vector>

#include "configuration/exception_source_information.hpp"

#include "core/option_map.hpp"
#include "core/option.hpp"
#include "core/parser.hpp"

#include "error/accessing_option_without_arguments.hpp"
#include "error/accessing_option_not_yet_added.hpp"
#include "error/unrecognized_option.hpp"

using namespace cli::core;

void
option_map::add_command_line_options(const parser::parsed_command_line& options)
{
    for (int i = 0, size = options.size(); i < size; ++i)
    {
	if (is_option_name(options[i]))
	{
	    add_option(options[i]);
	}

	else
	{
	    add_option_argument(options[i - 1], options[i]);
	}
    }
}

void option_map::add_option(std::string_view option_name)
{
    if (not contains(option_name))
    {
	std::string_view key = option_name;
	std::string_view value;

	bool long_option_with_argument = false;

	if (is_long_option_name_with_argument(option_name))
	{
	    auto position = option_name.find('=');

	    key   = option_name.substr(0, position);
	    value = option_name.substr(++position);

	    long_option_with_argument = true;
	}

	if (dictionary_contains_option(key))
	{
	    map.emplace_back(value_type {key, {}});

	    if (long_option_with_argument)
	    {
		auto arguments = split_arguments(value);

		if (arguments.empty())
		{
		    // raise an exception

		    add_option_argument(key, {});
		}

		for (auto&& argument : arguments)
		{
		    add_option_argument(key, argument);
		}
	    }
	}

	else
	{
	    throw std::logic_error {
		std::string(__func__)
		    .append(": unrecognized option ")
		    .append(option_name)
	    };
	}
    }
}

void option_map::add_option_argument(std::string_view option_name,
				     std::string_view option_argument)
{
    add_option(option_name);

    auto& option = get_option_from_dictionary(option_name);

    if (option.has_arguments())
    {
	if (option_argument.empty())
	{
	    throw std::logic_error {
		std::string(__func__)
		    .append(": ")
		    .append(option_name)
		    .append(" expects argument")
	    };
	}

	get_option_arguments(option_name)
	    .emplace_back(option_argument);
    }

    else
    {
	throw std::logic_error {
	    std::string(__func__)
		.append(": ")
		.append(option_name)
		.append(" does not take arguments")
        };
    }
}

const option_map::mapped_type&
option_map::operator[](const option& option) const
{
    if (auto it = find_option_in_map(option); it != map.cend())
    {
	if (option.has_arguments())
	{
	    return it->second;
	}

	throw error::accessing_option_without_arguments {
	    option.short_name().empty() ?
	    option.long_name() :
	    option.short_name(),
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    if (dictionary_contains_option(option))
    {
	throw error::accessing_option_not_yet_added {
	    option.short_name().empty() ?
	    option.long_name() :
	    option.short_name(),
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    throw error::unrecognized_option {
	option.short_name().empty() ?
	option.long_name() :
	option.short_name(),
	EXCEPTION_SOURCE_INFORMATION
    };
}

const option_map::mapped_type&
option_map::operator[](std::string_view option_name) const
{
    if (auto it = find_option_in_map(option_name); it != map.cend())
    {
	if (get_option_from_dictionary(option_name).has_arguments())
	{
	    return it->second;
	}

	throw error::accessing_option_without_arguments {
	    option_name,
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    if (dictionary_contains_option(option_name))
    {
	throw error::accessing_option_not_yet_added {
	    option_name,
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    throw error::unrecognized_option {
	option_name,
        EXCEPTION_SOURCE_INFORMATION
    };
}

std::vector<std::string_view>
option_map::split_arguments(std::string_view option_argument)
{
    std::vector<std::string_view> arguments;

    for (int i = 0, size = option_argument.size(); i < size; ++i)
    {
	while (i < size && option_argument[i] == ',')
	{
	    ++i;
	}

	int first = i++;

	while (i < size && option_argument[i] != ',')
	{
	    ++i;
	}

	int last = i;

	auto argument = option_argument.substr(first, last - first);

	if (not argument.empty())
	{
	    arguments.emplace_back(argument);
	}
    }

    return arguments;
}
