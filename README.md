# cli

A lightweight C++ library for parsing command-line arguments with validation support and flexible configuration options.

## requirements

+ C++23 compatible compiler
+ CMake 3.11+

## build

```bash
    git clone git@github.com:kcherno/cli.git

    cd cli

    cmake -B build -S .

    cmake --build build
```

## quick start

<details>

<summary>basic usage</summary>

```c++
#include <exception>
#include <iostream>
#include <format>

#include "cli/options.hpp"
#include "cli/option.hpp"
#include "cli/parser.hpp"

int main(int argc, char** argv)
{
    try
    {
        cli::options options {
            cli::option {
                .short_name  = "-h",
                .long_name   = "--help"
            }
        };

        options.description("<application-name> options:");

        const auto& [parsed_options, positional_options] =
            cli::parser(options).parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            std::cout << std::format(
                "usage: <application-name> [<options>] [<arguments>]\n\n");

            std::cout << options;
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << std::format("<application-name>: {}\n", exception.what());

        return 1;
    }
}
```

</details>

<details>

<summary>option with arguments</summary>

```c++
#include <exception>
#include <iostream>
#include <format>

#include "cli/options.hpp"
#include "cli/option.hpp"
#include "cli/parser.hpp"

int main(int argc, char** argv)
{
    try
    {
        cli::options options {
            cli::option {
                .short_name    = "-e",
                .long_name     = "--edit",
                .description   = "edit the file",
                .has_arguments = true
            },

            cli::option {
                .long_name     = "--no-edit",
                .description   = "do not edit the file",
                .has_arguments = true
            },

            cli::option {
                .short_name    = "-f",
                .long_name     = "--file",
                .description   = "add input file",
                .has_arguments = true
            },

            cli::option {
                .short_name  = "-h",
                .long_name   = "--help",
                .description = "print help message"
            }
        };

        options.description("<application-name> options:");

        const auto& [parsed_options, positional_options] =
            cli::parser(options).parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            std::cout << std::format(
                "usage: <application-name> [<options>] [<arguments>]\n\n");

            std::cout << options;
        }

        else
        {
            ...
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << std::format("<application-name>: {}\n", exception.what());

        return 1;
    }
}
```

</details>

<details>

<summary>custom validation</summary>

```c++
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <format>

#include "cli/options.hpp"
#include "cli/option.hpp"
#include "cli/parser.hpp"

int main(int argc, char** argv)
{
    try
    {
        constexpr auto does_file_exist = [](std::string_view filename)
        {
            if (not std::filesystem::exists(filename))
            {
                throw std::runtime_error {
                    std::format("{} does not exist", filename)
                };
            }
        };

        cli::options options {
            cli::option {
                .short_name        = "-e",
                .long_name         = "--edit",
                .description       = "edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            cli::option {
                .long_name         = "--no-edit",
                .description       = "do not edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            cli::option {
                .short_name        = "-f",
                .long_name         = "--file",
                .description       = "add input file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            cli::option 
            {
                .short_name  = "-h",
                .long_name   = "--help",
                .description = "print help message"
            }
        };

        options.description("<application-name> options:");

        const auto& [parsed_options, positional_options] =
            cli::parser(options).parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            std::cout << std::format(
                "usage: <application-name> [<options>] [<arguments>]\n\n");

            std::cout << options;
        }

        else
        {
            ...
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << std::format("<application-name>: {}\n", exception.what());

        return 1;
    }
}
```

</details>

<details>

<summary>multiple options</summary>

```c++
#include <string_view>
#include <filesystem>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <format>

#include "cli/options.hpp"
#include "cli/option.hpp"
#include "cli/parser.hpp"

int main(int argc, char** argv)
{
    try
    {
        cli::options options {
            cli::option {
                .short_name             = "-h",
                .long_name              = "--help",
                .description            = "print help message",
                .has_arguments          = true,
                .are_arguments_required = false,
                .argument_verifier      = [](std::string_view argument)
                {
                    if (argument != "filesystem")
                    {
                        throw std::runtime_error {
                            std::format(
                                "the {} is an invalid option group name\n"
                                "\n"
                                "list of options group:\n"
                                "\n"
                                "filesystem: working with files",
                                argument
                            )
                        };
                    }    
                }
            }
        };

        options.description("<appication-name> options:");

        constexpr auto does_file_exist = [](std::string_view filename)
        {
            if (not std::filesystem::exists(filename))
            {
                throw std::runtime_error {
                    std::format("{} file does not exist", filename)
                };
            }
        };

        cli::options filesystem_options {
            cli::option {
                .short_name        = "-e",
                .long_name         = "--edit",
                .description       = "edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            cli::option {
                .long_name         = "--no-edit",
                .description       = "do not edit the file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            },

            cli::option {
                .short_name        = "-f",
                .long_name         = "--file",
                .description       = "add input file",
                .has_arguments     = true,
                .argument_verifier = does_file_exist
            }
        };

        filesystem_options.description("filesystem options:");

        const auto& [parsed_options, positional_options] =
            cli::parser({options, filesystem_options})
                .parse_command_line(argc, argv);

        if (parsed_options.contains("-h"))
        {
            const auto& arguments = parsed_options["-h"];

            if (arguments.empty())
            {
                std::cout << std::format(
                    "usage: <application-name> [<options>] [<arguments>]\n\n");
                
                std::cout << options
                          << '\n'
                          << filesystem_options
                          << '\n'
                          << "try using <help> followed by the name of "
                             "an option group to display only the options "
                             "of that group"
                          << std::endl;
            }

            else if (arguments.front() == "filesystem")
            {
                std::cout << filesystem_options;
            }
        }

        else
        {
            ...
        }
    }

    catch (const std::exception& exception)
    {
        std::cerr << std::format("<application-name>: {}\n", exception.what());

        return 1;
    }
}
```

</details>

## API documentation

### cli::option

#### types

| Type | Definition |
| --- | --- |
| `verifier_type` | `void(*)(std::string_view)` |

#### data members

| Member | Type | Default initialization | Semantics |
| --- | :---: | :---: | --- |
| `short_name` | `std::string_view` | `{}` | Short option name, e.g., `-f` |
| `long_name` | `std::string_view` | `{}` | Long option name, e.g., `--file` |
| `description` | `std::string_view` | `{}` | Option description |
| `is_required` | `bool` | `false` | Specifies whether the option must be present in the command line |
| `has_arguments` | `bool` | `false` | Specifies whether the option has arguments |
| `are_arguments_required` | `bool` | `true` | Specifies whether the option must receive an argument on the command line |
| `maximum_number_of_arguments` | `unsigned int` | `0 or std::numeric_limits<unsigned int>::max()` | Specifies the maximum number of the option arguments |
| `argument_verifier` | `verifier_type` | `nullptr` | Function to validate a command line argument. ***The function must throw an exception on validation failure*** |

> [!TIP]
> *There is no need to initialize all fields. Use only what you need.*

#### member functions

| Expression | Return type | Semantics |
| --- | :---: | --- |
| `representation()` | `std::string` | Returns the option representation for output, e.g., `-f, --file` |

### cli::options

#### data members

Specifies a description for an option group.

```c++
    std::string description;
```

Specifies the number of spaces on the left when outputting options.

```c++
    unsigned int description_left_indent;
```

**Example:**

```c++
    ...

    cli::options::options {
        "<application-name> options:", {
            ...
        }
    };

    ...

    options.description_left_indent = 0;

    std::cout << options;

    // <application-name> options:
    //
    // -h, --help print help message

    options.description_left_indent = 10;

    std::cout << options;

    // <application-name> options:
    //
    //           -h, --help print help message

    ...
```

Specifies the number of new lines when outputting options after the header.

```c++
    unsigned int description_vertical_gap;
```

**Example:**

```c++
    ...

    cli::options::options {
        "<application-name> options:", {
            ...
        }
    };

    options.description_vertical_gap = 0;

    std::cout << options;

    // <application-name> options:
    //     -h, --help print help message

    options.description_vertical_gap = 5;

    std::cout << options;

    // <application-name> options:
    //
    //
    //
    //
    //
    //           -h, --help print help message

    ...
```

#### static data members

By default, sets the number of spaces on the left when outputting options.

```c++
    `static constexpr unsigned int` default_description_left_indent = 4;
```

By default, sets the number of new lines after the description when outputting options.

```c++
    `static constexpr unsigned int` default_description_vertical_gap = 1;
```

#### member functions

| Expression | Return Type | Semantics |
| --- | :---: | --- |
| `options()` | `options` | Default constructor, `description.empty()` is `true`, `description_left_indent` is equal `default_description_left_indent`, `description_vertical_gap` is equal `default_description_vertical_gap`, `options.empty()` is `true` |
| `options(std::string_view, std::initializer_list<option>)` | `options` | Specifies a description for an option group and initialize it using an initializer list. |
| `options(const options&)` | `options` | Copy constructor |
| `options(options&&)` | `options` | Move constructor |
| `operator=(const options&)` | `options&` | Copy assignment operator |
| `operator=(options&&)` | `options&` | Move assignment operator |
| `add_option(const option&)` | `void` | Add the option to the options |
| `contains(std::string_view)` | `bool` | Checks whether an option is contained by short or long name |
| `operator[](std::string_view)` | `const option&` | Returns a const reference to the option by its name |

### cli::parser

#### types

| Type | Definition |
| --- | --- |
| `pair` | `std::pair<parsed_options, positional_options>` |

#### member functions

| Expression | Return type | Semantics |
| --- | :---: | --- |
| `parser()` | `parser` | Default constructor |
| `parser(const options&)` | `parser` | Initialize the parser with the options |
| `parser(std::initializer_list<options>)` | `parser` | Initialize the parser with a list of optionss |
| `parser(const parser&)` | `parser` | Copy constructor |
| `parser(parser&&)` | `parser` | Move constructor |
| `operator=(const parser&)` | `parser&` | Copy assignment operator |
| `operator=(parser&&)` | `parser&` | Move assignment operator |
| `parse_command_line(int, char**)` | `parser::pair` | Parses command line and returns parsed options and positional options |
| `parse_command_line(int, const char**)` | `parser::pair` | Parses command line and returns parsed options and positional options |

### cli::parsed_options

#### types

| Type | Definition |
| --- | --- |
| `vector` | `std::vector<std::string_view>` |

#### member functions

| Expression | Return type | Semantics |
| --- | :---: | --- |
| `parsed_options()` | `parsed_options` | Default constructor |
| `parsed_options(parsed_options&&)` | `parsed_options` | Move constructor |
| `operator=(parsed_options&&)` | `parsed_options` | Move assignment operator |
| `contains(std::string_view)` | `bool` | Checks whether an option is contained by short or long name |
| `operator[](std::string_view)` | `const parsed_options::vector&` | Returns a list of arguments for the option |
