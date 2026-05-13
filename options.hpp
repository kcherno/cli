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

        using list = std::list<option>;

        using unordered_map =
            std::unordered_map<std::string_view, list::const_iterator>;

        options() = default;

        options(std::string_view description) :
            description_ {description}
        {}

        options(std::initializer_list<option> initializer_list)
        {
            for (auto&& option : initializer_list)
            {
                add_option(option);
            }
        }

        options(const options& other) :
            description_ {other.description_}
        {
            for (auto&& option : other.list_)
            {
                add_option(option);
            }
        }

        options(options&& other) noexcept(
            std::is_nothrow_move_constructible_v<std::string> &&
            std::is_nothrow_move_constructible_v<list> &&
            std::is_nothrow_move_constructible_v<unordered_map>)
                : description_   {std::move(other.description_)},
                  list_          {std::move(other.list_)},
                  unordered_map_ {std::move(other.unordered_map_)}
        {}

        options& operator=(const options& other) noexcept
        {
            if (this != &other)
            {
                description_ = other.description_;

                for (auto&& option : other.list_)
                {
                    add_option(option);
                }
            }

            return *this;
        }

        options& operator=(options&& other) noexcept(
            std::is_nothrow_swappable_v<std::string> &&
            std::is_nothrow_swappable_v<list> &&
            std::is_nothrow_swappable_v<unordered_map>)
        {
            if (this != &other)
            {
                std::swap(description_,   other.description_);
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

        std::string_view description() const noexcept
        {
            return description_;
        }

        void description(std::string_view description)
        {
            description_ = description;
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

        inline static constinit std::size_t left_indent  = 4;
        inline static constinit std::size_t vertical_gap = 1;

    private:

        std::string   description_;
        list          list_;
        unordered_map unordered_map_;
    };

    std::ostream& operator<<(std::ostream&, const options&);
}
