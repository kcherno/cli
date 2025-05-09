#include <string_view>
#include <utility>

#include "configuration/exception_source_information.hpp"

#include "core/option.hpp"

#include "error/option_must_have_at_least_short_or_long_name.hpp"
#include "error/invalid_format_for_short_option_name.hpp"
#include "error/invalid_format_for_long_option_name.hpp"

using namespace cli::core;

option::option(
    std::string_view               short_name,
    std::string_view               long_name,
    std::string_view               representation,
    std::string_view               description,
    required                       is_required,
    arguments                      has_arguments,
    const equality_validator_type& equality_validator)
    :
    short_name_         {short_name},
    long_name_          {long_name},
    representation_     {representation},
    description_        {description},
    is_required_        {is_required},
    has_arguments_      {has_arguments},
    equality_validator_ {equality_validator}
{
    if (not (short_name.empty() || is_short_option_name(short_name)))
    {
	throw error::invalid_format_for_short_option_name {
	    short_name,
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    if (not (long_name.empty() || is_long_option_name(long_name)))
    {
	throw error::invalid_format_for_long_option_name {
	    long_name,
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    if (short_name.empty() && long_name.empty())
    {
	throw error::option_must_have_at_least_short_or_long_name {
	    EXCEPTION_SOURCE_INFORMATION
	};
    }
}

option::option(option&& other) noexcept :
    short_name_         {other.short_name_},
    long_name_          {other.long_name_},
    representation_     {other.representation_},
    description_        {other.description_},
    is_required_        {other.is_required_},
    has_arguments_      {other.has_arguments_},
    equality_validator_ {std::move(other.equality_validator_)}
{
    other.short_name_     = {};
    other.long_name_      = {};
    other.representation_ = {};
    other.description_    = {};
    other.is_required_    = required::not_required;
    other.has_arguments_  = arguments::no_arguments;
}

option& option::operator=(option&& other) noexcept
{
    if (this != &other)
    {
	std::swap(short_name_,         other.short_name_);
	std::swap(long_name_,          other.long_name_);
	std::swap(representation_,     other.representation_);
	std::swap(description_,        other.description_);
	std::swap(is_required_,        other.is_required_);
	std::swap(has_arguments_,      other.has_arguments_);
	std::swap(equality_validator_, other.equality_validator_);
    }

    return *this;
}

void option::short_name(std::string_view other)
{
    if (other.empty() && long_name_.empty())
    {
	throw error::option_must_have_at_least_short_or_long_name {
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    if (other.empty() || is_short_option_name(other))
    {
	short_name_ = other;
    }

    else
    {
	throw error::invalid_format_for_short_option_name {
	    other,
	    EXCEPTION_SOURCE_INFORMATION
	};
    }
}

void option::long_name(std::string_view other)
{
    if (other.empty() && short_name_.empty())
    {
	throw error::option_must_have_at_least_short_or_long_name {
	    EXCEPTION_SOURCE_INFORMATION
	};
    }

    if (other.empty() || is_long_option_name(other))
    {
	long_name_ = other;
    }

    else
    {
	throw error::invalid_format_for_long_option_name {
	    other,
	    EXCEPTION_SOURCE_INFORMATION
	};
    }
}
