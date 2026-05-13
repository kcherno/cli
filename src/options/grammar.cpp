#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <iomanip>
#include <ostream>
#include <string>

#include <cctype>

#include "options/is_short_option_name.hpp"
#include "options/is_long_option_name.hpp"
#include "options/grammar.hpp"
#include "options/option.hpp"

void options::grammar::add_option(const option& option)
{
    verify(option);

    auto iterator_1 = map_.find(option.short_name);
    auto iterator_2 = map_.find(option.long_name);

    if (iterator_1 != map_.end() || iterator_2 != map_.end())
    {
        throw std::logic_error {
            std::string("grammar::add_option: ")
                .append("option [")
                .append(option.representation())
                .append("] has already been added as [")
                .append(iterator_1 == map_.end() ?
                    iterator_2->second->representation() :
                    iterator_1->second->representation())
                .append("]")
        };
    }

    list_.push_back(option);

    if (not option.short_name.empty())
    {
        map_[option.short_name] = std::prev(list_.end());
    }

    if (not option.long_name.empty())
    {
        map_[option.long_name] = std::prev(list_.end());
    }
}

void options::grammar::verify(const option& option)
{
    if (option.short_name.empty() && option.long_name.empty())
    {
        throw std::logic_error {
            "grammar::verify: "
            "the option must have at least a short or long name"
        };
    }

    if ((not option.short_name.empty()) &&
        (not is_short_option_name(option.short_name)))
    {
        throw std::logic_error {
            "grammar::verify: "
            "the short option name must match the pattern ^-[a-zA-Z0-9]$"
        };
    }

    if ((not option.long_name.empty()) &&
        (not is_long_option_name(option.long_name)))
    {
        throw std::logic_error {
            "grammar::verify: "
            "the long option name must match the pattern "
            "^--[a-zA-Z][-a-zA-Z0-9]*[a-zA-Z]$"
        };
    }
}

std::ostream& options::operator<<(
    std::ostream& ostream, const grammar& grammar)
{
    if (not grammar.description().empty())
    {
        ostream << grammar.description() << '\n'
                << std::string(grammar.vertical_gap, '\n');
    }

    const auto gap =
        std::ranges::max_element(grammar, [](auto&& lhs, auto&& rhs)
        {
            return lhs.long_name.size() < rhs.long_name.size();
        })->long_name.size();

    std::for_each(grammar.begin(), std::prev(grammar.end()), [&](auto&& option)
    {
        ostream << std::string(grammar.left_indent, ' ')
                << option.short_name
                << (option.short_name.empty() ? "    " :
                        (option.long_name.empty() ? "  " : ", "))
                << std::setw(gap)
                << std::left
                << option.long_name
                << ' '
                << option.description
                << '\n';
    });

    const auto& option = *std::prev(grammar.end());

    return ostream << std::string(grammar.left_indent, ' ')
                   << option.short_name
                   << (option.short_name.empty() ? "    " :
                           (option.long_name.empty() ? "  " : ", "))
                   << std::setw(gap)
                   << std::left
                   << option.long_name
                   << ' '
                   << option.description;
}
