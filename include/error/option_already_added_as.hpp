#pragma once

#include <string_view>
#include <string>

#include "generic/exception.hpp"

namespace cli::error
{
    class option_already_added_as final : public generic::exception
    {
    public:

	option_already_added_as(
            std::string_view option,
	    std::string_view added_as,
	    std::string_view where = {})
	    :
	    generic::exception {
		std::string(option)
		    .append(" already added as ")
		    .append(added_as),
		where
	    }
	{}
    };
}
