#pragma once

#include <initializer_list>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

#include "option.hpp"

namespace cli::core
{
    class option_book final
    {
    public:

	option_book() = default;

	option_book(std::initializer_list<option> ilist) :
	    options {ilist}
	{}

	option_book(const option_book&) = default;

	option_book(option_book&& other) noexcept :
	    option_book {}
	{
	    this->operator=(std::move(other));
	}

	option_book& operator=(const option_book&) = default;

	option_book& operator=(option_book&& other) noexcept
	{
	    if (this != &other)
	    {
		std::swap(options, other.options);
	    }

	    return *this;
	}

	bool contains(std::string_view option_name) const noexcept
	{
	    if (find_option(option_name) != options.cend())
	    {
		return true;
	    }

	    return false;
	}

	const option& operator[](std::string_view option_name) const
	{
	    auto iterator = find_option(option_name);

	    if (iterator != options.cend())
	    {
		return *iterator;
	    }

	    throw std::out_of_range {__func__};
	}

	option& operator[](std::string_view option_name)
	{
	    using const_this = const option_book*;

	    auto&& const_reference =
		const_cast<const_this>(this)->operator[](option_name);

	    return const_cast<option&>(const_reference);
	}

	bool operator==(const option_book& other) const noexcept
	{
	    return options == other.options;
	}

	bool operator!=(const option_book& other) const noexcept
	{
	    return options != other.options;
	}

    private:

	std::vector<option>::const_iterator
	find_option(std::string_view option_name) const noexcept
	{
	    return std::find(options.cbegin(), options.cend(), option_name);
	}

	std::vector<option> options;
    };
}
