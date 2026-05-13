#pragma once

#include <string_view>
#include <algorithm>

#include <cctype>

namespace options
{
    inline constexpr bool is_long_option_name(std::string_view name) noexcept
    {
        if (name.size() > 3 && name.starts_with("--") && std::isalpha(name[2]))
        {
            constexpr auto predicate = [](char ch)
            {
                return ch == '-' || std::isalnum(ch);
            };

            auto substr = name.substr(3);

            if (std::ranges::all_of(substr, predicate))
            {
                return not substr.contains("--") &&
                    std::isalnum(substr.back());
            }
        }

        return false;
    }
}
