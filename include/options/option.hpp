#pragma once

#include <string_view>
#include <string>

namespace options
{
    struct option final
    {
        using verifier_type = void(*)(std::string_view);

        std::string_view short_name             = {};
        std::string_view long_name              = {};
        std::string_view description            = {};
        bool             is_required            = false;
        bool             has_arguments          = false;
        bool             are_arguments_required = true;
        verifier_type    argument_verifier      = nullptr;

        std::string representation() const
        {
            std::string representation;

            representation += short_name;

            representation += short_name.empty() ? "" : ", ";

            representation += long_name;

            if (long_name.empty())
            {
                representation.resize(short_name.size());
            }

            return representation;
        }
    };
}
