#pragma once

#include <string_view>
#include <limits>
#include <string>

namespace cli
{
    struct option final
    {
        using verifier_type = void(*)(std::string_view);

        std::string_view short_name                  = {};
        std::string_view long_name                   = {};
        std::string_view description                 = {};
        bool             is_required                 = false;
        bool             has_arguments               = false;
        bool             are_arguments_required      = true;

        unsigned int     maximum_number_of_arguments = has_arguments ?
            std::numeric_limits<decltype(maximum_number_of_arguments)>::max() :
            0;

        verifier_type    argument_verifier           = nullptr;

        std::string representation() const;
    };
}
