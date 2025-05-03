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
	    if (auto iterator = find(option_name); iterator != options.cend())
	    {
		return true;
	    }

	    return false;
	}

	const option& operator[](std::string_view option_name) const
	{
	    if (auto iterator = find(option_name); iterator != options.cend())
	    {
		return *iterator;
	    }

	    throw std::out_of_range {__func__};
	}

	option& operator[](std::string_view option_name)
	{
	    using const_this = option_book*;

	    auto&& reference =
		const_cast<const_this>(this)->operator[](option_name);

	    return const_cast<option&>(reference);
	}

    private:

	std::vector<option>::const_iterator
	find(std::string_view option_name) const noexcept
	{
	    return std::find(options.cbegin(), options.cend(), option_name);
	}

	std::vector<option> options;
    };
}
