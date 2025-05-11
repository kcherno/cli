#pragma once

#include <string_view>
#include <string>

#include "generic/exception.hpp"

namespace cli::error
{
    class accessing_option_without_arguments final : public generic::exception
    {
    public:

	accessing_option_without_arguments(
            std::string_view option, std::string_view where = {}) :
	    generic::exception {
		std::string("accessing ")
		    .append(option)
		    .append(" without arguments"),
		where
	    }
	{}
    };
}
