#include <string_view>
#include <vector>

#include "core/parser.hpp"

#include "error/long_option_starts_with_hyphen.hpp"
#include "error/long_option_with_one_hyphen.hpp"
#include "error/short_option_empty.hpp"
#include "error/long_option_empty.hpp"

using namespace cli::core;

std::vector<std::string_view>
parser::parse_command_line_options(int argc, const char** argv) const
{
    std::vector<std::string_view> options;

    for (int i = 1; i < argc && argv[i]; ++i)
    {
	auto option = std::string_view {argv[i]};

	if (not option.empty() && option[0] == '-')
	{
	    switch (option.size())
	    {
	    case 1:

		throw error::short_option_empty {__func__};

	    case 2:

		if (option[1] == '-')
		{
		    throw error::long_option_empty {__func__};
		}

		break;

	    default:

		if (option[1] != '-')
		{
		    throw error::long_option_with_one_hyphen {__func__};
		}

		if (option[2] == '-')
		{
		    throw error::long_option_starts_with_hyphen {__func__};
		}
	    }

	    options.emplace_back(option);
	}
    }

    return options;
}

std::vector<std::string_view>
parser::parse_positional_options(int argc, const char** argv) const
{
    std::vector<std::string_view> options;

    for (int i = 1; i < argc && argv[i]; ++i)
    {
	const auto option = std::string_view {argv[i]};

	if (option.empty() || option[0] == '-')
	{
	    continue;
	}

	options.emplace_back(option);
    }

    return options;
}
