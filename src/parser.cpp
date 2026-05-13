#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <utility>
#include <string>
#include <vector>

#include "detail/verification.hpp"

#include "parsed_options.hpp"
#include "parser.hpp"

namespace
{
    inline constexpr bool is_long_option_name_with_argument(
        std::string_view name) noexcept
    {
        if (auto position = name.find('='); position != name.npos)
        {
            return cli::detail::is_long_option_name(name.substr(0, position));
        }

        return false;
    }

    inline constexpr bool is_option_name(std::string_view name) noexcept
    {
        return cli::detail::is_short_option_name(name) ||
            cli::detail::is_long_option_name(name) ||
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

std::pair<cli::parsed_options, cli::positional_options>
cli::parser::parse_command_line(int argc, const char** argv) const
{
    parsed_options parsed_options;

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

                option = option_name;

                argument = option_argument;
            }

            else
            {
                option = argv[i];
            }

            if (not contains(option))
            {
                throw std::runtime_error {
                    std::format("{} is an unrecognized option", option)
                };
            }

            auto&& opt = operator[](option);

            if (opt.has_arguments)
            {
                if (not is_long_option_name_with_argument(argv[i]))
                {
                    if (argv[i + 1] != nullptr)
                    {
                        argument = argv[++i];
                    }
                }

                if (opt.are_arguments_required && argument.empty())
                {
                    throw std::runtime_error {
                        std::format(
                            "[{}] expects an argument", opt.representation())
                    };
                }

                if (opt.argument_verifier && not argument.empty())
                {
                    opt.argument_verifier(argument);
                }
            }

            if (not parsed_options.contains(option))
            {
                parsed_options.add_option(opt);
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

    std::ranges::for_each(vector_, [&parsed_options](auto&& options)
    {
        std::ranges::for_each(options, [&parsed_options](auto&& option)
        {
            auto option_name = option.short_name.empty() ?
                option.long_name : option.short_name;

            if (option.is_required && not parsed_options.contains(option_name))
            {
                throw std::runtime_error {
                    std::format("[{}] is required", option.representation())
                };
            }
        });
    });

    return std::pair {
        std::move(parsed_options),
        std::move(positional_options)
    };
}
