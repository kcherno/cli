#pragma once

#include <initializer_list>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

#include "configuration/exception_source_information.hpp"

#include "option.hpp"

namespace cli::core
{
    class dictionary final
    {
    public:

	using container = std::vector<option>;

	using value_type      = container::value_type;
	using reference       = container::reference;
	using const_reference = container::const_reference;
	using iterator        = container::iterator;
	using const_iterator  = container::const_iterator;
	using difference_type = container::difference_type;
	using size_type       = container::size_type;

	dictionary() = default;

	dictionary(std::initializer_list<option> initializer_list) :
	    container_ {initializer_list}
	{}

	dictionary(const dictionary&) = default;

	dictionary(dictionary&& other) noexcept :
	    container_ {std::move(other.container_)}
	{}

	dictionary& operator=(const dictionary&) = default;

	dictionary& operator=(dictionary&& other) noexcept
	{
	    if (this != &other)
	    {
		swap(other);
	    }

	    return *this;
	}

	const_iterator cbegin() const noexcept
	{
	    return container_.cbegin();
	}

	const_iterator begin() const noexcept
	{
	    return container_.begin();
	}

	iterator begin() noexcept
	{
	    return container_.begin();
	}

	bool contains(const_reference option) const noexcept
	{
	    return find_option(option) != container_.cend();
	}

	bool contains(std::string_view option_name) const noexcept
	{
	    return find_option(option_name) != container_.cend();
	}

	bool empty() const noexcept
	{
	    return container_.empty();
	}

	const_iterator cend() const noexcept
	{
	    return container_.cend();
	}

	const_iterator end() const noexcept
	{
	    return container_.end();
	}

	iterator end() noexcept
	{
	    return container_.end();
	}

	size_type max_size() const noexcept
	{
	    return container_.max_size();
	}

	size_type size() const noexcept
	{
	    return container_.size();
	}

	void swap(dictionary& other) noexcept
	{
	    container_.swap(other.container_);
	}

	const_reference operator[](const_reference option) const
	{
	    auto iterator = find_option(option);

	    if (iterator != container_.cend())
	    {
		return *iterator;
	    }

	    throw std::out_of_range {EXCEPTION_SOURCE_INFORMATION};
	}

	reference operator[](const_reference option)
	{
	    using const_this = const dictionary*;

	    auto&& const_reference =
		const_cast<const_this>(this)->operator[](option);

	    return const_cast<reference>(const_reference);
	}

	const_reference operator[](std::string_view option_name) const
	{
	    auto iterator = find_option(option_name);

	    if (iterator != container_.cend())
	    {
		return *iterator;
	    }

	    throw std::out_of_range {EXCEPTION_SOURCE_INFORMATION};
	}

	reference operator[](std::string_view option_name)
	{
	    using const_this = const dictionary*;

	    auto&& const_reference =
		const_cast<const_this>(this)->operator[](option_name);

	    return const_cast<reference>(const_reference);
	}

    private:

	const_iterator find_option(const_reference option) const noexcept
	{
	    return std::find(container_.cbegin(), container_.cend(), option);
	}

	const_iterator find_option(std::string_view option_name) const noexcept
	{
	    return std::find(
                container_.cbegin(), container_.cend(), option_name);
	}

        container container_;
    };

    inline bool operator==(
        const dictionary& lhs, const dictionary& rhs) noexcept
    {
	return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    }

    inline bool operator!=(
        const dictionary& lhs, const dictionary& rhs) noexcept
    {
	return (not (lhs == rhs));
    }
}
