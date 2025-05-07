#pragma once

#include <string_view>
#include <string>

#include "generic/exception.hpp"

namespace cli::error
{
    class unrecognized_option final : public generic::exception
    {
    public:

	unrecognized_option(std::string_view option = {},
			    std::string_view where  = {})
	    :
	    generic::exception {
		option.empty() ?
		option :
		std::string("unrecognized option").append(" ").append(option),

		where
	    }
	{}
    };
}
