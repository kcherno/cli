#pragma once

#include <initializer_list>
#include <string_view>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <vector>

#include "positional_options.hpp"
#include "parsed_options.hpp"
#include "options.hpp"
#include "option.hpp"

namespace cli
{
    class parser final
    {
    public:

        // alias for parsed and positional options
        using pair = std::pair<parsed_options, positional_options>;

        using vector = std::vector<options>;

        parser() = default;

        parser(const parser& other) noexcept(
            std::is_nothrow_copy_constructible_v<vector>)
                : vector_ {other.vector_}
        {}

        parser(parser&& other) noexcept(
            std::is_nothrow_move_constructible_v<vector>)
                : vector_ {std::move(other.vector_)}
        {}

        parser(std::initializer_list<options> options) noexcept(
            std::is_nothrow_constructible_v<vector, decltype(options)>)
                : vector_ {options}
        {}

        parser(const options& options) noexcept(
            std::is_nothrow_constructible_v<vector, decltype(options)>)
                : vector_ {options}
        {}

        parser& operator=(const parser& other) noexcept(
            std::is_nothrow_copy_assignable_v<vector>)
        {
            if (this != &other)
            {
                vector_ = other.vector_;
            }

            return *this;
        }

        parser& operator=(parser&& other) noexcept(
            std::is_nothrow_swappable_v<vector>)
        {
            if (this != &other)
            {
                std::swap(vector_, other.vector_);
            }

            return *this;
        }

        constexpr void clear() noexcept
        {
            vector_.clear();
        }

        constexpr bool empty() const noexcept
        {
            return vector_.empty();
        }

        pair parse_command_line(int argc, char** argv) const
        {
            return parse_command_line(argc, const_cast<const char**>(argv));
        }

        pair parse_command_line(int, const char**) const;

    private:

        auto find(
            this auto&& self, std::string_view option_name) noexcept
        {
            return std::ranges::find_if(self.vector_, [=](auto&& options)
            {
                return options.contains(option_name);
            });
        }

        bool contains(std::string_view option_name) const noexcept
        {
            return find(option_name) != vector_.end();
        }

        const option& operator[](std::string_view option_name) const & noexcept
        {
            return find(option_name)->operator[](option_name);
        }

        vector vector_;
    };
}
