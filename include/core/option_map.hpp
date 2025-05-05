#pragma once

#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

#include "option_book.hpp"
#include "option.hpp"

namespace cli::core
{
    class option_map final
    {
    public:

	using key_type        = std::string_view;
	using mapped_type     = std::vector<std::string_view>;
	using value_type      = std::pair<key_type, mapped_type>;
	using reference       = value_type&;
	using const_reference = const value_type&;

	option_map() = default;

	option_map(std::initializer_list<option_book> ilist) :
	    books {ilist}
	{}

	void add_command_line_options(std::vector<std::string_view>);

	void add_option(std::string_view);

	void add_option_argument(std::string_view, std::string_view);

	void add_option_book(const option_book& option_book)
	{
	    if (not contains(option_book))
	    {
		books.emplace_back(std::move(option_book));
	    }
	}

	void add_option_book(option_book&& option_book) noexcept
	{
	    if (not contains(option_book))
	    {
		books.emplace_back(std::move(option_book));
	    }
	}

	bool contains(const option_book& option_book) const noexcept
	{
	    auto iterator =
		std::find(books.cbegin(), books.cend(), option_book);

	    return iterator != books.cend();
	}

	bool contains(std::string_view option_name) const noexcept
	{
	    auto iterator =
		std::find_if(map.cbegin(), map.cend(), [&](auto&& value)
		{
		    return value.first == option_name;
		});

	    return iterator != map.cend();
	}

	const mapped_type& operator[](std::string_view) const;

	bool empty() const noexcept
	{
	    return map.empty();
	}

    private:

	bool book_contains_option(std::string_view option_name) const noexcept
	{
	    return std::find_if(books.cbegin(), books.cend(), [&](auto&& book)
	    {

		return book.contains(option_name);

	    }) != books.cend();
	}

	bool
	contains_with_validation(std::string_view option_name) const noexcept
	{
	    return find_with_validation(option_name) != map.cend();
	}

	std::vector<value_type>::const_iterator
	find_with_validation(std::string_view option_name) const noexcept
	{
	    if (book_contains_option(option_name))
	    {
		auto& option = get_option_from_book(option_name);

		return std::find_if(map.cbegin(), map.cend(), [&](auto&& value)
		{
		    return option == value.first;
		});
	    }

	    return map.cend();
	}

	const mapped_type&
	get_option_arguments(std::string_view option_name) const noexcept
	{
	    return find_with_validation(option_name)->second;
	}

	mapped_type&
	get_option_arguments(std::string_view option_name) noexcept
	{
	    using const_this = const option_map*;

	    auto& arguments =
		const_cast<const_this>(this)->get_option_arguments(option_name);

	    return const_cast<mapped_type&>(arguments);
	}

	const option&
	get_option_from_book(std::string_view option_name) const noexcept
	{
	    return std::find_if(books.begin(), books.end(), [&](auto&& book)
	    {

		return book.contains(option_name);

	    })->operator[](option_name);
	}

	static std::vector<std::string_view> split_arguments(std::string_view);

	std::vector<option_book> books;
	std::vector<value_type>  map;
    };
}
