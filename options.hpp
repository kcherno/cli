#pragma once

#include <initializer_list>
#include <unordered_map>
#include <string_view>
#include <type_traits>
#include <stdexcept>
#include <ostream>
#include <utility>
#include <format>
#include <string>
#include <list>

#include "option.hpp"

namespace cli
{
    class options final
    {
    public:

        struct detail final
        {
            using list          = std::list<option>;

            using unordered_map =
                std::unordered_map<std::string_view, list::const_iterator>;

            detail() = delete;
        };

        std::string  description;
        unsigned int description_left_indent;
        unsigned int description_vertical_gap;

        static constexpr unsigned int default_description_left_indent  = 4;
        static constexpr unsigned int default_description_vertical_gap = 1;

        options() :
            description              {},
            description_left_indent  {default_description_left_indent},
            description_vertical_gap {default_description_vertical_gap},
            list_                    {},
            unordered_map_           {}
        {}

        options(
            std::string_view              description,
            std::initializer_list<option> initializer_list)
                : description              {description},
                  description_left_indent  {default_description_left_indent},
                  description_vertical_gap {default_description_vertical_gap}
        {
            for (auto&& option : initializer_list)
            {
                add_option(option);
            }
        }

        options(const options& other) :
            description              {other.description},
            description_left_indent  {other.description_left_indent},
            description_vertical_gap {other.description_vertical_gap}
        {
            for (auto&& option : other.list_)
            {
                add_option(option);
            }
        }

        options(options&& other) noexcept(
            std::is_nothrow_move_constructible_v<std::string> &&
            std::is_nothrow_move_constructible_v<detail::list> &&
            std::is_nothrow_move_constructible_v<detail::unordered_map>)
                : description              {std::move(other.description)},
                  description_left_indent  {other.description_left_indent},
                  description_vertical_gap {other.description_vertical_gap},
                  list_                    {std::move(other.list_)},
                  unordered_map_           {std::move(other.unordered_map_)}
        {
            other.description_left_indent  = default_description_left_indent;
            other.description_vertical_gap = default_description_vertical_gap;
        }

        options& operator=(const options& other) noexcept
        {
            if (this != &other)
            {
                description              = other.description;
                description_left_indent  = other.description_left_indent;
                description_vertical_gap = other.description_vertical_gap;

                for (auto&& option : other.list_)
                {
                    add_option(option);
                }
            }

            return *this;
        }

        options& operator=(options&& other) noexcept(
            std::is_nothrow_swappable_v<std::string> &&
            std::is_nothrow_swappable_v<detail::list> &&
            std::is_nothrow_swappable_v<detail::unordered_map>)
        {
            if (this != &other)
            {
                std::swap(description, other.description);

                std::swap(
                    description_left_indent, other.description_left_indent);

                std::swap(
                    description_vertical_gap, other.description_vertical_gap);

                std::swap(list_,          other.list_);
                std::swap(unordered_map_, other.unordered_map_);
            }

            return *this;
        }

        void add_option(const option&);

        constexpr auto begin(this auto&& self)
        {
            return self.list_.begin();
        }

        bool contains(std::string_view option_name) const noexcept
        {
            if (empty())
            {
                return false;
            }

            return unordered_map_.contains(option_name);
        }

        constexpr bool empty() const noexcept
        {
            return list_.empty();
        }

        constexpr auto end(this auto&& self)
        {
            return self.list_.end();
        }

        constexpr std::size_t size() const noexcept
        {
            return list_.size();
        }

        const option& operator[](std::string_view option_name) const &
        {
            auto iterator = unordered_map_.find(option_name);

            if (iterator != unordered_map_.end())
            {
                return *(iterator->second);
            }

            throw std::runtime_error {
                std::format("{} was not added", option_name)
            };
        }

        friend std::ostream& operator<<(std::ostream&, const options&);

    private:

        detail::list          list_;
        detail::unordered_map unordered_map_;
    };

    std::ostream& operator<<(std::ostream&, const options&);
}
