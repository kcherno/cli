#pragma once

#include <string_view>

#include "generic/exception.hpp"

namespace cli::error
{
    class long_option_starts_with_hyphen final : public generic::exception
    {
    public:

	long_option_starts_with_hyphen(std::string_view what = {}) :
	    generic::exception {"long option starts with '-'", what}
	{}
    };
}
