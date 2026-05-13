#pragma once

#include <initializer_list>
#include <unordered_map>
#include <string_view>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <vector>
#include <list>

#include "options/grammar.hpp"
#include "options/option.hpp"

namespace options
{
    class parser final
    {
    public:

        class parsed_options final
        {
        public:

            friend parser;

            using options_list = std::list<
                std::pair<option, std::vector<std::string_view>>>;

            using options_map = std::unordered_map<
                std::string_view, options_list::iterator>;

            parsed_options(const parsed_options&) = delete;

            parsed_options(parsed_options&& other) noexcept(
                std::is_nothrow_move_constructible_v<options_list> &&
                std::is_nothrow_move_constructible_v<options_map>) :
                list_ {std::move(other.list_)},
                map_  {std::move(other.map_)}
            {}

            parsed_options& operator=(const parsed_options&) = delete;

            parsed_options& operator=(parsed_options&& other) noexcept(
                std::is_nothrow_swappable_v<options_list> &&
                std::is_nothrow_swappable_v<options_map>)
            {
                if (this != &other)
                {
                    std::swap(list_, other.list_);
                    std::swap(map_,  other.map_);
                }

                return *this;
            }

            bool contains(std::string_view option_name) const noexcept
            {
                return map_.contains(option_name);
            }

            constexpr bool empty() const noexcept
            {
                return list_.empty();
            }

            constexpr std::size_t size() const noexcept
            {
                return list_.size();
            }

            const std::vector<std::string_view>& operator[](
                std::string_view) const;

        private:

            parsed_options() = default;

            void add_option(const option&);

            std::vector<std::string_view>& operator[](
                std::string_view option_name) noexcept
            {
                return map_[option_name]->second;
            }

            options_list list_;
            options_map  map_;
        };

        using positional_options = std::vector<std::string_view>;

        parser() = default;

        parser(const grammar& grammar) noexcept(
            std::is_nothrow_constructible_v<
                parser, std::initializer_list<options::grammar>>) :
            parser {{grammar}}
        {}

        parser(std::initializer_list<grammar> grammars) noexcept(
            std::is_nothrow_constructible_v<
                std::vector<grammar>, std::initializer_list<grammar>>) :
            grammars_ {grammars}
        {}

        parser(const parser& other) noexcept(
            std::is_nothrow_copy_constructible_v<std::vector<grammar>>) :
            grammars_ {other.grammars_}
        {}

        parser(parser&& other) noexcept(
            std::is_nothrow_move_constructible_v<std::vector<grammar>>) :
            grammars_ {std::move(other.grammars_)}
        {}

        parser& operator=(const parser& other) noexcept(
            std::is_nothrow_copy_assignable_v<std::vector<grammar>>)
        {
            if (this != &other)
            {
                grammars_ = other.grammars_;
            }

            return *this;
        }

        parser& operator=(parser&& other) noexcept(
            std::is_nothrow_swappable_v<std::vector<grammar>>)
        {
            if (this != &other)
            {
                std::swap(grammars_, other.grammars_);
            }

            return *this;
        }

        std::pair<parsed_options, positional_options>
        parse_command_line(int argc, char** argv) const
        {
            return parse_command_line(argc, const_cast<const char**>(argv));
        }

        std::pair<parsed_options, positional_options>
        parse_command_line(int, const char**) const;

    private:

        auto find(
            this auto&& self, std::string_view option_name) noexcept
        {
            return std::ranges::find_if(self.grammars_, [=](auto&& grammar)
            {
                return grammar.contains(option_name);
            });
        }

        bool contains(std::string_view option_name) const noexcept
        {
            return find(option_name) != grammars_.end();
        }

        const option& operator[](std::string_view option_name) const noexcept
        {
            return find(option_name)->operator[](option_name);
        }

        std::vector<grammar> grammars_;
    };
}
