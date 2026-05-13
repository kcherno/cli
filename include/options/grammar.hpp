#pragma once

#include <initializer_list>
#include <unordered_map>
#include <string_view>
#include <type_traits>
#include <stdexcept>
#include <ostream>
#include <utility>
#include <string>
#include <list>

#include "option.hpp"

namespace options
{
    class grammar final
    {
    public:

        using value_type      = option;
        using reference       = option&;
        using const_reference = const option&;
        using iterator        = std::list<option>::iterator;
        using const_iterator  = std::list<option>::const_iterator;
        using difference_type = std::list<option>::difference_type;
        using size_type       = std::list<option>::size_type;

        grammar() = default;

        grammar(std::string_view description) :
            description_ {description}
        {}

        grammar(std::initializer_list<option> options)
        {
            for (auto&& option : options)
            {
                add_option(option);
            }
        }

        grammar(const grammar& other) :
            description_ {other.description_}
        {
            for (auto&& option : other.list_)
            {
                add_option(option);
            }
        }

        grammar(grammar&& other) noexcept(
            std::is_nothrow_move_constructible_v<std::string> &&
            std::is_nothrow_move_constructible_v<std::list<option>> &&
            std::is_nothrow_move_constructible_v<std::unordered_map<
                std::string_view, std::list<option>::const_iterator>>) :
            description_ {std::move(other.description_)},
            list_        {std::move(other.list_)},
            map_         {std::move(other.map_)}
        {}

        grammar& operator=(const grammar& other)
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

        grammar& operator=(grammar&& other) noexcept(
            std::is_nothrow_swappable_v<std::string> &&
            std::is_nothrow_swappable_v<std::list<option>> &&
            std::is_nothrow_swappable_v<std::unordered_map<
                std::string_view, std::list<option>::const_iterator>>)
        {
            if (this != &other)
            {
                std::swap(description_, other.description_);
                std::swap(list_, other.list_);
                std::swap(map_, other.map_);
            }

            return *this;
        }

        void add_option(const option&);

        auto begin() noexcept
        {
            return list_.begin();
        }

        auto begin() const noexcept
        {
            return list_.begin();
        }

        bool contains(std::string_view option_name) const noexcept
        {
            return map_.contains(option_name);
        }

        std::string_view description() const noexcept
        {
            return description_;
        }

        void description(std::string_view description)
        {
            description_ = description;
        }

        bool empty() const noexcept
        {
            return list_.empty();
        }

        auto end() noexcept
        {
            return list_.end();
        }

        auto end() const noexcept
        {
            return list_.end();
        }

        size_type size() const noexcept
        {
            return list_.size();
        }

        static void verify(const option&);

        const option& operator[](std::string_view option_name) const
        {
            if (auto iterator = map_.find(option_name); iterator != map_.end())
            {
                return *(iterator->second);
            }

            throw std::logic_error {
                std::string("grammar::operator[]: the ")
                    .append(option_name)
                    .append(" option name is not saved")
            };
        }

        inline static constinit std::size_t left_indent  = 4;
        inline static constinit std::size_t vertical_gap = 1;

    private:

        std::string                                          description_;
        std::list<option>                                    list_;
        std::unordered_map<std::string_view, const_iterator> map_;
    };

    std::ostream& operator<<(std::ostream&, const grammar&);
}
