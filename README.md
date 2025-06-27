# error handling library.

# installation

you may copy the files under LibError/include into your project, install it with [CPM](https://github.com/cpm-cmake/CPM.cmake) or install directly into your system with the following: 

* ``py install.py``

and then include it with cmake into your project

```cmake
cmake_minimum_required_version(VERSION 3.25)

project(CoolProject LANGUAGES CXX)

find_package(LibError CONFIG REQUIRED)
add_executable(CoolProject source.cpp)
target_link_libraries(CoolProject PRIVATE LibError::LibError)
```

# examples
```c++
#include <liberror/Result.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

liberror::Result<std::string> read_file_contents(std::filesystem::path path)
{
    std::ifstream fileStream { path };

    if (fileStream.fail())
    {
        return liberror::make_error("Couldn't read file: \"{}\"", path.string());
    }

    std::stringstream contentStream {};
    contentStream << fileStream.rdbuf();

    return contentStream.str();
}

liberror::Result<void> print_file_contents(std::filesystem::path path)
{
    // if the function fails, the program stops execution and prints the error.
    auto const contents = MUST(read_file_contents("some_cool_file.txt"));

    std::cout << "reading: {}" << path.string() << '\n';
    std::cout << "size in bytes: {}" << contents.size() << '\n';
    std::cout << "{}" << contents << '\n';

    return {}; // this is how you return from void
}

int main()
{
    auto result = print_file_contents("some_cool_file.txt");

    if (!result.has_value())
    {
        std::cerr << result.error().message() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

```c++
#include <liberror/Result.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

liberror::Result<std::string> read_file_contents(std::filesystem::path path)
{
    std::ifstream fileStream { path };

    if (fileStream.fail())
    {
        return liberror::make_error("Couldn't read file: \"{}\"", path.string());
    }

    std::stringstream contentStream {};
    contentStream << fileStream.rdbuf();

    return contentStream.str();
}

liberror::Result<void> print_file_contents(std::filesystem::path path)
{
    // if the function fails, the error is pushed up into the call stack to be handled somewhere else.
    auto const contents = TRY(read_file_contents("some_cool_file.txt"));

    std::cout << "reading: {}" << path.string() << '\n';
    std::cout << "size in bytes: {}" << contents.size() << '\n';
    std::cout << "{}" << contents << '\n';

    return {}; // this is how you return from void
}

int main()
{
    auto result = print_file_contents("some_cool_file.txt");

    if (!result.has_value())
    {
        std::cerr << result.error().message() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

i recommend you to simply explore the code and see what you can do with it. seriously. do it.
