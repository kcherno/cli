#pragma once

#include <string_view>

#include "generic/exception.hpp"

namespace cli::error
{
    class option_must_have_at_least_short_or_long_name final :
	public generic::exception
    {
    public:

	option_must_have_at_least_short_or_long_name(
            std::string_view where = {})
	    :
	    generic::exception {
		"option must have at least short or long name",
		where
	    }
	{}
    };
}
