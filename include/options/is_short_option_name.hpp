#pragma once

#include <string_view>

#include <cctype>

namespace options
{
    inline constexpr bool is_short_option_name(std::string_view name) noexcept
    {
        return name.size() == 2 && name[0] == '-' && std::isalnum(name[1]);
    }
}
