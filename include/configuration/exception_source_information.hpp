#pragma once

#ifndef DISABLE_EXCEPTION_SOURCE_INFORMATION

#include <source_location>
#include <string>

#define EXCEPTION_SOURCE_INFORMATION \
    std::string(std::source_location::current().file_name()) \
	.append(":") \
	.append(std::source_location::current().function_name()) \
	.append(":") \
	.append(std::to_string(std::source_location::current().line())) \
	.append(":") \
	.append(std::to_string(std::source_location::current().column()))

#else

#define EXCEPTION_SOURCE_INFORMATION ""

#endif
