#pragma once

#include <string_view>
#include <functional>
#include <utility>

namespace cli::core
{
    class option final
    {
    public:

	using equality_validator_type = std::function<bool(std::string_view)>;

	enum class required
	{
	    not_required = 0,
	    required
	};

	enum class arguments
	{
	    no_arguments = 0,
	    has_arguments
	};

	option(
            std::string_view = {},
	    std::string_view = {},
	    std::string_view = {},
	    std::string_view = {},
	    required  = required::not_required,
	    arguments = arguments::no_arguments,
	    const equality_validator_type& = {});

	option(const option&) = default;

	option(option&& other) noexcept :
	    option {}
	{
	    this->operator=(std::move(other));
	}

	option& operator=(const option&) = default;

	option& operator=(option&&) noexcept;

	constexpr std::string_view short_name() const noexcept
	{
	    return short_name_;
	}

	void short_name(std::string_view);

	constexpr std::string_view long_name() const noexcept
	{
	    return long_name_;
	}

	void long_name(std::string_view);

	constexpr std::string_view representation() const noexcept
	{
	    return representation_;
	}

	constexpr void representation(std::string_view other) noexcept
	{
	    representation_ = other;
	}

	constexpr std::string_view description() const noexcept
	{
	    return description_;
	}

	constexpr void description(std::string_view other) noexcept
	{
	    description_ = other;
	}

	constexpr bool is_required() const noexcept
	{
	    return is_required_ == required::required;
	}

	constexpr void is_required(required enumerator) noexcept
	{
	    is_required_ = enumerator;
	}

	constexpr bool has_arguments() const noexcept
	{
	    return has_arguments_ == arguments::has_arguments;
	}

	constexpr void has_arguments(arguments enumerator) noexcept
	{
	    has_arguments_ = enumerator;
	}

	bool has_equality_validator() const noexcept
	{
	    return static_cast<bool>(equality_validator_);
	}

	const equality_validator_type& equality_validator() const noexcept
	{
	    return equality_validator_;
	}

	void equality_validator(equality_validator_type&& other) noexcept
	{
	    std::swap(equality_validator_, other);
	}

	static constexpr bool
	is_short_option_name(std::string_view option_name) noexcept
	{
	    return option_name.size() == 2 && option_name.starts_with('-');
	}

	static constexpr bool
	is_long_option_name(std::string_view option_name) noexcept
	{
	    return option_name.size() > 2 && option_name.starts_with("--");
	}

	static constexpr bool
	is_long_option_name_with_argument(std::string_view option_name) noexcept
	{
	    if (is_long_option_name(option_name))
	    {
		return option_name.find('=') != std::string_view::npos;
	    }

	    return false;
	}

	static constexpr bool
	is_option_name(std::string_view option_name) noexcept
	{
	    return (is_short_option_name(option_name) ||
		    is_long_option_name(option_name)  ||
		    is_long_option_name_with_argument(option_name));
	}

    private:

	std::string_view short_name_;
	std::string_view long_name_;
	std::string_view representation_;
	std::string_view description_;

	required  is_required_;
	arguments has_arguments_;

	equality_validator_type equality_validator_;
    };

    inline bool operator==(const option& lhs, const option& rhs) noexcept
    {
	return lhs.short_name() == rhs.short_name() &&
	       lhs.long_name()  == rhs.long_name();
    }

    inline bool
    operator==(const option& option, std::string_view option_name) noexcept
    {
	if (option.has_equality_validator())
	{
	    return option.equality_validator()(option_name);
	}

	return (option.short_name() == option_name ||
		option.long_name()  == option_name);
    }

    inline bool
    operator==(std::string_view option_name, const option& option) noexcept
    {
	return option == option_name;
    }

    inline bool
    operator!=(const option& option, std::string_view option_name) noexcept
    {
	return (not (option == option_name));
    }

    inline bool
    operator!=(std::string_view option_name, const option& option) noexcept
    {
	return option != option_name;
    }
}
