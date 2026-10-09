#pragma once

#include <unordered_map>
#include <type_traits>
#include <string_view>
#include <utility>
#include <vector>
#include <list>

#include "option.hpp"

namespace cli
{
    class parser;

    class parsed_options final
    {
    public:

        friend parser;

        // container for storing option arguments
        using vector = std::vector<std::string_view>;

        using list =
            std::list<std::pair<option, vector>>;

        using unordered_map =
            std::unordered_map<std::string_view, list::iterator>;

        parsed_options() = default;

        parsed_options(const parsed_options&) = delete;

        parsed_options(parsed_options&& other) noexcept(
            std::is_nothrow_move_constructible_v<list> &&
            std::is_nothrow_move_constructible_v<unordered_map>)
                : list_          {std::move(other.list_)},
                  unordered_map_ {std::move(other.unordered_map_)}
        {}

        parsed_options& operator=(const parsed_options&) = delete;

        parsed_options& operator=(parsed_options&& other) noexcept(
            std::is_nothrow_swappable_v<list> &&
            std::is_nothrow_swappable_v<unordered_map>)
        {
            if (this != &other)
            {
                std::swap(list_,           other.list_);
                std::swap(unordered_map_,  other.unordered_map_);
            }

            return *this;
        }

        bool contains(std::string_view option_name) const noexcept
        {
            return unordered_map_.contains(option_name);
        }

        constexpr bool empty() const noexcept
        {
            return list_.empty();
        }

        constexpr std::size_t size() const noexcept
        {
            return list_.size();
        }

        const vector& operator[](std::string_view) const &;

    private:

        void add_option(const option&);

        vector& operator[](std::string_view option_name) & noexcept
        {
            return unordered_map_[option_name]->second;
        }

        list          list_;
        unordered_map unordered_map_;
    };
}
