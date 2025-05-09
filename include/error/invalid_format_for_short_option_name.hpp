#pragma once

#include <string_view>
#include <string>

#include "generic/exception.hpp"

namespace cli::error
{
    class invalid_format_for_short_option_name final : public generic::exception
    {
    public:

	invalid_format_for_short_option_name(std::string_view option,
					     std::string_view where = {})
	    :
	    generic::exception {
		std::string(option)
		    .append(" invalid format for short option name"),
		where
	    }
	{}
    };
}
