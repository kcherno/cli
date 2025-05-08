#pragma once

#include <string_view>
#include <string>

#include "generic/exception.hpp"

namespace cli::error
{
    class option_is_required_but_not_added final : public generic::exception
    {
    public:

	option_is_required_but_not_added(std::string_view option,
					 std::string_view where = {})
	    :
	    generic::exception {
		std::string(option).append(" is required but not added"),
		where
	    }
	{}
    };
}
