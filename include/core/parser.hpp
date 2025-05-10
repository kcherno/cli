#pragma once

#include <initializer_list>
#include <string_view>
#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

#include "core/dictionary.hpp"
#include "core/option.hpp"

namespace cli::core
{
    class parser final
    {
    public:

	class parsed_command_line final : private std::vector<std::string_view>
	{
	public:

	    friend parser;

	    using container = std::vector<std::string_view>;

	    using value_type      = container::value_type;
	    using reference       = container::reference;
	    using const_reference = container::const_reference;
	    using size_type       = container::size_type;
	    using iterator        = container::iterator;
	    using const_iterator  = container::const_iterator;

	    const_iterator cbegin() const noexcept
	    {
		return container::cbegin();
	    }

	    const_iterator cend() const noexcept
	    {
		return container::cend();
	    }

	    const_iterator begin() const noexcept
	    {
		return container::begin();
	    }

	    const_iterator end() const noexcept
	    {
		return container::end();
	    }

	    bool empty() const noexcept
	    {
		return container::empty();
	    }

	    size_type size() const noexcept
	    {
		return container::size();
	    }

	    const_reference operator[](size_type position) const
	    {
		return container::operator[](position);
	    }

	private:

	    parsed_command_line() = default;
	};

	parser() = default;

	parser(std::initializer_list<dictionary> dictionaries)
	{
	    for (auto&& dictionary : dictionaries)
	    {
		add_dictionary(dictionary);
	    }
	}

	parser(const parser&) = default;

	parser(parser&& other) noexcept :
	    parser {}
	{
	    this->operator=(std::move(other));
	}

	parser& operator=(const parser&) = default;

	parser& operator=(parser&& other) noexcept
	{
	    if (this != &other)
	    {
		std::swap(dictionaries,               other.dictionaries);
		std::swap(options_,            other.options_);
		std::swap(positional_options_, other.positional_options_);
	    }

	    return *this;
	}

	void add_dictionary(const dictionary& dictionary)
	{
	    if (not contains(dictionary))
	    {
		dictionaries.emplace_back(dictionary);
	    }
	}

	void add_dictionary(dictionary&& dictionary)
	{
	    if (not contains(dictionary))
	    {
		dictionaries.emplace_back(std::move(dictionary));
	    }
	}

	void clear() noexcept
	{
	    dictionaries.clear();
	}

	std::optional<std::string_view>
	contains(const option& option) const noexcept
	{
	    auto iterator =
		std::find(options_.cbegin(), options_.cend(), option);

	    if (iterator != options_.cend())
	    {
		return *iterator;
	    }

	    return {};
	}

	bool contains(const dictionary& dictionary) const noexcept
	{
	    auto iterator =
		std::find(dictionaries.cbegin(), dictionaries.cend(), dictionary);

	    return iterator != dictionaries.cend();
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

	void erase(const dictionary& dictionary)
	{
	    auto iterator =
		std::remove(dictionaries.begin(), dictionaries.end(), dictionary);

	    dictionaries.erase(iterator, dictionaries.end());
	}

	bool empty() const noexcept
	{
	    return dictionaries.empty();
	}

	const parsed_command_line& options() const noexcept
	{
	    return options_;
	}

	void parse_command_line(int, const char**);

	void parse_command_line(int argc, char** argv)
	{
	    parse_command_line(argc, const_cast<const char**>(argv));
	}

	const std::vector<std::string_view>& positional_options() const noexcept
	{
	    return positional_options_;
	}

    private:

	void add_option(std::string_view);

	bool dictionary_contains(std::string_view option_name) const noexcept
	{
	    return find_option_in_dictionary(option_name) != dictionaries.cend();
	}

	std::vector<dictionary>::const_iterator
	find_option_in_dictionary(std::string_view option_name) const noexcept
	{
	    return std::find_if(dictionaries.cbegin(), dictionaries.cend(), [&](auto&& dictionary)
	    {
		return dictionary.contains(option_name);
	    });
	}

	std::vector<std::string_view>::const_iterator
	find_option_with_validation(std::string_view) const noexcept;

	const option&
	get_option_from_dictionary(std::string_view option_name) const noexcept
	{
	    return find_option_in_dictionary(option_name)->operator[](option_name);
	}

	std::vector<dictionary> dictionaries;

	parsed_command_line           options_;
	std::vector<std::string_view> positional_options_;
    };
}
