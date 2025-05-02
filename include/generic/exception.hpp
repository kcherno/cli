#pragma once

#include <string_view>
#include <exception>
#include <string>

namespace cli::generic
{
    class exception : public std::exception
    {
    public:

	exception(std::string_view error, std::string_view what = {}) :
	    what_ {
		what.empty() ?
		error        :
		std::string(what).append(": ").append(error)
	    }
	{}

	virtual ~exception() = default;

	const char* what() const noexcept override
	{
	    return what_.data();
	}

    private:

	std::string what_;
    };
}
