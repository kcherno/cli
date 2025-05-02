#pragma once

#include <string_view>
#include <utility>
#include <vector>

namespace cli::core
{
    class parser final
    {
    public:

	auto parse_command_line(int argc, const char** argv) const
	{
	    return std::make_pair(
                parse_command_line_options(argc, argv),
                parse_positional_options(argc, argv)
            );
	}

	auto parse_command_line(int argc, char** argv) const
	{
	    return parse_command_line(argc, const_cast<const char**>(argv));
	}

	std::vector<std::string_view>
	parse_command_line_options(int, const char**) const;

	std::vector<std::string_view>
	parse_command_line_options(int argc, char** argv) const
	{
	    return parse_command_line_options(
                argc,
		const_cast<const char**>(argv)
            );
	}

	std::vector<std::string_view>
	parse_positional_options(int, const char**) const;

	std::vector<std::string_view>
	parse_positional_options(int argc, char** argv) const
	{
	    return parse_positional_options(
                argc,
		const_cast<const char**>(argv)
            );
	}
    };
}
