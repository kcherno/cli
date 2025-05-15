# 1 Overview

A lightweight CLI parsing library that provides a simple way to handle command line arguments.

# 2 Requirements

- C++20 or higher
- Boost.Test framework (optional, only required for running tests)

# 3 Installation

```bash

git clone git@github.com:kcherno/cli.git

cd cli

# create build directory and configure
cmake -S . -B build

# build the project
cmake --build build

# run tests (optional)
cmake --test-dir build

```

# 4 Usage

## 4.1 Creating a simple option

```c++

option help {
    "-h",
    "--help"
};

```

> *Note: An option must have at least a short or long name*

## 4.2 Creating an option with a validator

For example, to handle complex options like `--verbose` or `--no-verbose`:

```c++

option verbose {
    "-v",
    "--verbose",
    "-v, --[no-]verbose",
    "verbose mode",
    option::required::not_required,  // not required during CLI parsing
    option::arguments::no_arguments, // has no arguments
    [](auto&& option_name) {
        return (option_name == "-v" ||
                option_name == "--verbose" ||
                option_name == "--no-verbose");
    }
};

```

## 4.3 Creating an option with arguments

```c++

option file {
    "-f",
    "--file",
    "-f, --file",
    "add file",
    option::required::not_required,
    option::arguments::has_arguments
};

```

Arguments can be passed in these formats:

- -f a.txt
- --file b.txt
- --file=c.txt,d.txt (will be divided into c.txt and d.txt)

## 4.4 Storing options in dictionaries

```c++

dictionary general_options {file, help, verbose};

```

## 4.5 Processing CLI with parser

```c++

parser parser {general_options};

parser.parse_command_line(argc, argv);

// Access parsed data

parser.options();            // returns parsed_command_line for option_map
parser.positional_options(); // returns positional options

```

## 4.6 Storing option arguments with option_map

```c++

option_map map {general_options};

map.add_command_line_options(parser.options());

```

### 4.6.1 Accessing arguments

```c++

map[file];     // return arguments by option
map["-f"];     // return arguments by short name
map["--file"]; // return arguments by long name

```

> *Note: All of these methods return `std::vector<std::string_view>`*

### 4.6.2 Checking for option presence

```c++

map.contains(verbose);
map.contains("-v");
map.contains("--verbose");
map.contains("--no-verbose");

```

> *Note: All these methods return `std::optional<std::string_view>`. To check for specific options:*

```c++

if (auto optional = map.contains("-v"); optional)
{
    if (optional.value() == "--no-verbose")
    {
        // Handle no-verbose case
    }
}

```
