#pragma once

#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <optional>
#include <utility>
#include <vector>

#include "dictionary.hpp"
#include "option.hpp"
#include "parser.hpp"

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

	option_map(std::initializer_list<dictionary> ilist) :
	    dictionaries {ilist}
	{}

	void add_command_line_options(const parser::parsed_command_line&);

	void add_dictionary(const dictionary& dictionary)
	{
	    if (not (dictionary.empty() || contains(dictionary)))
	    {
		dictionaries.emplace_back(dictionary);
	    }
	}

	void add_dictionary(dictionary&& dictionary) noexcept
	{
	    if (not (dictionary.empty() || contains(dictionary)))
	    {
		dictionaries.emplace_back(std::move(dictionary));
	    }
	}

	bool contains(const dictionary& dictionary) const noexcept
	{
	    if (not dictionary.empty())
	    {
		auto iterator = std::find(
                    dictionaries.cbegin(), dictionaries.cend(), dictionary);

		return iterator != dictionaries.cend();
	    }

	    return false;
	}

	bool contains(const option& option) const noexcept
	{
	    auto iterator = std::find_if(
                map.cbegin(), map.cend(), [&](auto&& value)
		{
		    return option == value.first;
		});

	    return iterator != map.cend();
	}

	std::optional<std::string_view>
	contains(std::string_view option_name) const noexcept
	{
	    auto iterator = find_option_in_map(option_name);

	    if (iterator != map.cend())
	    {
		return iterator->first;
	    }

	    return {};
	}

	const mapped_type& operator[](std::string_view) const;

	bool empty() const noexcept
	{
	    return dictionaries.empty();
	}

    private:

	void add_option(std::string_view);

	void add_option_argument(std::string_view, std::string_view);

	bool
	dictionary_contains_option(std::string_view option_name) const noexcept
	{
	    return (find_option_in_dictionary(option_name) !=
		    dictionaries.cend());
	}

	std::vector<dictionary>::const_iterator
	find_option_in_dictionary(std::string_view option_name) const noexcept
	{
	    return std::find_if(
                dictionaries.cbegin(),
		dictionaries.cend(),
		[&](auto&& dictionary)
		{
		    return dictionary.contains(option_name);
		});
	}

	std::vector<value_type>::const_iterator
	find_option_in_map(std::string_view option_name) const noexcept
	{
	    if (dictionary_contains_option(option_name))
	    {
		auto& option = get_option_from_dictionary(option_name);

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
	    return find_option_in_map(option_name)->second;
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
	get_option_from_dictionary(std::string_view option_name) const noexcept
	{
	    return (*find_option_in_dictionary(option_name))[option_name];
	}

	static std::vector<std::string_view> split_arguments(std::string_view);

	std::vector<dictionary> dictionaries;
	std::vector<value_type> map;
    };
}
