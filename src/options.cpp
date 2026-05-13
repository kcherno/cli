#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <iomanip>
#include <ostream>
#include <format>
#include <string>

#include <cctype>

#include "detail/verification.hpp"

#include "options.hpp"
#include "option.hpp"

void cli::options::add_option(const option& option)
{
    detail::verify(option);

    auto iterator_1 = unordered_map_.find(option.short_name);
    auto iterator_2 = unordered_map_.find(option.long_name);

    if (iterator_1 != unordered_map_.end() ||
        iterator_2 != unordered_map_.end())
    {
        throw std::runtime_error {
            std::format(
                "option [{}] has already been added as [{}]",
                option.representation(),
                iterator_1 == unordered_map_.end() ?
                    iterator_2->second->representation() :
                    iterator_1->second->representation()
            )
        };
    }

    list_.push_back(option);

    if (not option.short_name.empty())
    {
        unordered_map_[option.short_name] = std::prev(list_.end());
    }

    if (not option.long_name.empty())
    {
        unordered_map_[option.long_name] = std::prev(list_.end());
    }
}

std::ostream& cli::operator<<(
    std::ostream& ostream, const options& options)
{
    if (not options.description_.empty())
    {
        ostream << options.description_
                << std::string(options.vertical_gap, '\n')
                << '\n';
    }

    const auto gap =
        std::ranges::max_element(options.list_, [](auto&& lhs, auto&& rhs)
        {
            return lhs.long_name.size() < rhs.long_name.size();
        }
    )->long_name.size();

    const auto left_indent = std::string(options.left_indent, ' ');

    std::ranges::for_each(options.list_, [&](auto&& option)
    {
        ostream << left_indent
                << option.short_name
                << (option.short_name.empty() ? "    " :
                       (option.long_name.empty() ? "  " : ", "))
                << std::setw(gap)
                << std::left
                << option.long_name
                << ' '
                << option.description
                << std::endl;
    });

    return ostream;
}
