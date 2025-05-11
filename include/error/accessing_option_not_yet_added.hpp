#pragma once

#include <string_view>
#include <string>

#include "generic/exception.hpp"

namespace cli::error
{
    class accessing_option_not_yet_added final : public generic::exception
    {
    public:

	accessing_option_not_yet_added(
            std::string_view option, std::string_view where = {}) :
	    generic::exception {
		std::string("accessing ")
		    .append(option)
		    .append(" not yet added"),
		where
	    }
	{}
    };
}
