#pragma once

#include <string_view>

#include "generic/exception.hpp"

namespace cli::error
{
    class long_option_empty final : public generic::exception
    {
    public:

	long_option_empty(std::string_view what = {}) :
	    generic::exception {"'--' without option name", what}
	{}
    };
}
