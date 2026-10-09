#include <string_view>
#include <algorithm>
#include <stdexcept>

#include <cctype>

#include "detail/verification.hpp"

#include "option.hpp"

bool cli::detail::is_long_option_name(std::string_view name) noexcept
{
    if (name.size() > 3 && name.starts_with("--"))
    {
        const auto substr = name.substr(3);

        const auto result = std::ranges::all_of(substr, [](char ch)
        {
            return ch == '-' || std::isalnum(ch);
        });

        return result &&
            not substr.contains("--") &&
            not substr.ends_with('-');
    }

    return false;
}

void cli::detail::verify(const option& option)
{
    if (option.short_name.empty() && option.long_name.empty())
    {
        throw std::runtime_error {
            "option must have at least a short or long name"
        };
    }

    if (not option.short_name.empty())
    {
        if (not is_short_option_name(option.short_name))
        {
            throw std::runtime_error {
                "short option name must match the pattern ^-[a-zA-Z0-9]$"
            };
        }
    }
    
    if (not option.long_name.empty())
    {
        if (not is_long_option_name(option.long_name))
        {
            throw std::runtime_error {
                "long option name must match the pattern "
                "^--[a-zA-Z0-9][-a-zA-Z0-9]*[a-zA-Z0-9]$"
            };
        }
    }
}
