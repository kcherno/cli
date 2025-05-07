#pragma once

#include <string_view>
#include <exception>
#include <string>

namespace cli::generic
{
    class exception : public std::exception
    {
    public:

	exception() = default;

	exception(std::string_view what) :
	    exception {what, ""}
	{}

	exception(std::string_view what, std::string_view where) :
	    what_ {
		where.empty() ?
		what          :
		std::string(where).append(": ").append(what)
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
