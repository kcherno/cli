#include <string_view>
#include <stdexcept>
#include <utility>
#include <format>
#include <vector>

#include "parsed_options.hpp"
#include "option.hpp"

void cli::parsed_options::add_option(const option& option)
{
    list_.push_back(std::pair {option, vector {}});

    if (not option.short_name.empty())
    {
        unordered_map_[option.short_name] = std::prev(list_.end());
    }

    if (not option.long_name.empty())
    {
        unordered_map_[option.long_name] = std::prev(list_.end());
    }
}

const cli::parsed_options::vector&
cli::parsed_options::operator[](std::string_view option_name) const &
{
    auto iterator = unordered_map_.find(option_name);

    if (iterator == unordered_map_.end())
    {
        throw std::runtime_error {
            std::format("{} was not recognized", option_name)
        };
    }

    const auto& [option, arguments] = *(iterator->second);

    if (not option.has_arguments)
    {
        throw std::runtime_error {
            std::format("[{}] has no arguments", option.representation())
        };
    }

    return arguments;
}
