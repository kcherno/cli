#pragma once

#include <string_view>
#include <string>

#include "generic/exception.hpp"

namespace cli::error
{
    class option_expects_argument final : public generic::exception
    {
    public:

	option_expects_argument(std::string_view option,
				std::string_view where = {})
	    :
	    generic::exception {
		std::string(option).append(" expects argument"),
		where
	    }
	{}
    };
}
