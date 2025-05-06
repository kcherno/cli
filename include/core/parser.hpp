#pragma once

#include <initializer_list>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <optional>
#include <utility>
#include <string>
#include <vector>

#include "core/option_book.hpp"
#include "core/option.hpp"

namespace cli::core
{
    class parser final
    {
    public:

	parser() = default;

	parser(std::initializer_list<option_book> books)
	{
	    for (auto&& book : books)
	    {
		add_option_book(book);
	    }
	}

	parser(const parser&) = default;

	parser(parser&& other);

	parser& operator=(const parser&) = default;

	parser& operator=(parser&& other) noexcept
	{
	    if (this != &other)
	    {
		std::swap(books, other.books);

		std::swap(options_,            other.options_);
		std::swap(positional_options_, other.positional_options_);
	    }

	    return *this;
	}

	void add_option_book(const option_book& option_book)
	{
	    if (not contains(option_book))
	    {
		books.emplace_back(option_book);
	    }
	}

	void add_option_book(option_book&& option_book)
	{
	    if (not contains(option_book))
	    {
		books.emplace_back(std::move(option_book));
	    }
	}

	void clear() noexcept
	{
	    books.clear();
	}

	bool contains(const option_book& option_book) const noexcept
	{
	    auto iterator =
		std::find(books.cbegin(), books.cend(), option_book);

	    return iterator != books.cend();
	}

	std::optional<std::string_view>
	contains(std::string_view option_name) const noexcept
	{
	    auto iterator = find_option_with_validation(option_name);

	    if (iterator != options_.cend())
	    {
		return *iterator;
	    }

	    return {};
	}

	void erase(const option_book& option_book)
	{
	    auto iterator =
		std::remove(books.begin(), books.end(), option_book);

	    books.erase(iterator, books.end());
	}

	bool empty() const noexcept
	{
	    return books.empty();
	}

	void parse_command_line(int, const char**);

	void parse_command_line(int argc, char** argv)
	{
	    parse_command_line(argc, const_cast<const char**>(argv));
	}

	const std::vector<std::string_view>& options() const noexcept
	{
	    return options_;
	}

	const std::vector<std::string_view>& positional_options() const noexcept
	{
	    return positional_options_;
	}

    private:

	void add_option(std::string_view option_name)
	{
	    auto option = option_name.substr(0, option_name.find('='));

	    if (book_contains(option))
	    {
		if (not get_option_from_book(option).has_arguments())
		{
		    if (auto optional = contains(option); optional)
		    {
			throw std::logic_error {
			    std::string(__func__)
				.append(": ")
				.append(option)
				.append(" already added as ")
				.append(optional.value())
			};
		    }
		}

		options_.emplace_back(option_name);
	    }

	    else
	    {
		throw std::logic_error {
		    std::string(__func__)
			.append(": ")
			.append(option_name)
			.append(" unrecognized option")
                };
	    }
	}

	void add_option_argument(std::string_view option_name,
				 std::string_view option_argument)
	{
	    if (get_option_from_book(option_name).has_arguments())
	    {
		options_.emplace_back(option_argument);
	    }

	    else
	    {
		throw std::logic_error {
		    std::string(__func__)
			.append(": ")
			.append(option_name)
			.append(" does not take arguments")
		};
	    }
	}

	bool book_contains(std::string_view option_name) const noexcept
	{
	    return find_option_in_book(option_name) != books.cend();
	}

	std::vector<option_book>::const_iterator
	find_option_in_book(std::string_view option_name) const noexcept
	{
	    return std::find_if(books.cbegin(), books.cend(), [&](auto&& book)
	    {
		return book.contains(option_name);
	    });
	}

	std::vector<std::string_view>::const_iterator
	find_option_with_validation(std::string_view option_name) const noexcept
	{
	    auto iterator = find_option_in_book(option_name);

	    if (iterator != books.cend())
	    {
		auto& option = iterator->operator[](option_name);

		return std::find_if(options_.cbegin(),
				    options_.cend(),
				    [&](auto&& op)
				    {
					return option == op;
				    });
	    }

	    return options_.cend();
	}

	const option&
	get_option_from_book(std::string_view option_name) const noexcept
	{
	    return find_option_in_book(option_name)->operator[](option_name);
	}

	std::vector<option_book> books;

	std::vector<std::string_view> options_;
	std::vector<std::string_view> positional_options_;
    };
}
