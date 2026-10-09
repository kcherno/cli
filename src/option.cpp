#include <string>

#include "option.hpp"

std::string cli::option::representation() const
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
