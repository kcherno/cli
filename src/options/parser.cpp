#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <utility>
#include <string>
#include <vector>

#include "options/is_short_option_name.hpp"
#include "options/is_long_option_name.hpp"
#include "options/parser.hpp"

void options::parser::parsed_options::add_option(const option& option)
{
    list_.push_back(std::pair {option, positional_options {}});

    if (not option.short_name.empty())
    {
        map_[option.short_name] = std::prev(list_.end());
    }

    if (not option.long_name.empty())
    {
        map_[option.long_name] = std::prev(list_.end());
    }
}

const std::vector<std::string_view>&
options::parser::parsed_options::operator[](std::string_view option_name) const
{
    auto iterator = map_.find(option_name);

    if (iterator == map_.end())
    {
        throw std::runtime_error {
            std::string("parser::parsed_options::operator[]: ")
                .append(option_name)
                .append(" is not stored")
        };
    }

    const auto& [option, arguments] = *(iterator->second);

    if (not option.has_arguments)
    {
        throw std::logic_error {
            std::string("parser::parsed_options::operator[]: [")
                .append(option.representation())
                .append("] has no arguments")
        };
    }

    return arguments;
}

namespace
{
    inline constexpr bool is_long_option_name_with_argument(
        std::string_view name) noexcept
    {
        if (auto position = name.find('='); position != name.npos)
        {
            return options::is_long_option_name(name.substr(0, position));
        }

        return false;
    }

    inline constexpr bool is_option_name(std::string_view name) noexcept
    {
        return options::is_short_option_name(name) ||
            options::is_long_option_name(name) ||
            is_long_option_name_with_argument(name);
    }

    inline constexpr std::pair<std::string_view, std::string_view>
    split_long_option_name_and_argument(std::string_view name) noexcept
    {
        const auto position = name.find('=');

        if (position == name.size() - 1)
        {
            return std::pair {name.substr(0, position), std::string_view {}};
        }

        return std::pair {name.substr(0, position), name.substr(position + 1)};
    }
}

std::pair<options::parser::parsed_options, options::parser::positional_options>
options::parser::parse_command_line(int argc, const char** argv) const
{
    parsed_options     parsed_options;
    positional_options positional_options;

    for (int i = 1; i < argc && argv[i]; ++i)
    {
        if (is_option_name(argv[i]))
        {
            std::string_view option;
            std::string_view argument;

            if (is_long_option_name_with_argument(argv[i]))
            {
                const auto& [option_name, option_argument] =
                    split_long_option_name_and_argument(argv[i]);

                option   = option_name;
                argument = option_argument;
            }

            else
            {
                option = argv[i];
            }

            if (not contains(option))
            {
                throw std::runtime_error {
                    std::string("parser::parse_command_line: ")
                        .append(option)
                        .append(" is an unrecognized option")
                };
            }

            if (operator[](option).has_arguments)
            {
                if (not is_long_option_name_with_argument(argv[i]))
                {
                    if (argv[i + 1] != nullptr)
                    {
                        argument = argv[++i];
                    }
                }

                if (operator[](option).are_arguments_required &&
                        argument.empty())
                {
                    throw std::runtime_error {
                        std::string("parser::parse_command_line: ")
                            .append(option)
                            .append(" expects an argument")
                    };
                }

                if (operator[](option).argument_verifier &&
                        not argument.empty())
                {
                    operator[](option).argument_verifier(argument);
                }
            }

            if (not parsed_options.contains(option))
            {
                parsed_options.add_option(operator[](option));
            }

            if (not argument.empty())
            {
                parsed_options[option].push_back(argument);
            }
        }

        else
        {
            positional_options.push_back(argv[i]);
        }
    }

    std::ranges::for_each(grammars_, [&parsed_options](auto&& grammar)
    {
        std::ranges::for_each(grammar, [&parsed_options](auto&& option)
        {
            std::string_view option_name = option.short_name.empty() ?
                option.long_name : option.short_name;

            if (option.is_required && not parsed_options.contains(option_name))
            {
                throw std::runtime_error {
                    std::string("parser::parse_command_line: [")
                        .append(option.representation())
                        .append("] is required")
                };
            }
        });
    });

    return std::pair {
        std::move(parsed_options),
        std::move(positional_options)
    };
}
