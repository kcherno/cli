#pragma once

#include <string_view>

#include "generic/exception.hpp"

namespace cli::error
{
    class short_option_empty final : public generic::exception
    {
    public:

	short_option_empty(std::string_view what = {}) :
	    generic::exception {"'-' without option name", what}
	{}
    };
}
