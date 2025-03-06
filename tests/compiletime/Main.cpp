#include <gtest/gtest.h>

#include <liberror/Result.hpp>

#include <fmt/format.h>

#include <memory>

using namespace liberror;

struct S
{
    explicit constexpr S(std::string value) : value_m { value } { fmt::println("S::S(std::string)"); }
    // cppcheck-suppress noExplicitConstructor
    constexpr S(char const* value) : value_m { value } { fmt::println("S::S(char const*)"); }
    constexpr ~S() noexcept { fmt::println("S::~S()"); }
    constexpr S(S const& s) : value_m { s.value_m } { fmt::println("S::S(S const&)"); }
    constexpr S(S&& s) noexcept : value_m { std::move(s.value_m) } { fmt::println("S::S(S&&)"); }
    constexpr S& operator=(S const& s) { value_m = s.value_m; fmt::println("S::S operator=(S const&)"); return *this; }
    constexpr S& operator=(S&& s) noexcept { value_m = std::move(s.value_m); fmt::println("S::S operator=(S&&)"); return *this; }

    std::string value_m {};
};

#ifdef __cpp_lib_expected
consteval Result<size_t> inspect_string(std::string_view input, size_t current = 0)
{
    size_t index = current;

    for (; index < input.size() && input.at(index) != '>'; index += 1)
    {
        if (input.at(index) == '<')
        {
            index += 1;
            auto maybeIndex = inspect_string(input, index);
            if (!maybeIndex.has_value()) { return make_error(maybeIndex.error()); }
            index = maybeIndex.value();
            if (index >= input.size() || input.at(index) != '>') return make_error("invalid string");
        }
    }

    return index;
}

TEST(compile_time, inspect_string_success)
{
    static_assert(inspect_string("<69420>").has_value() == true);
    static_assert(inspect_string("<<69420> is <42069>>").has_value() == true);
}

TEST(compile_time, inspect_string_failure)
{
    static_assert(inspect_string("<69420").has_value() == false);
    static_assert(inspect_string("<69420 is <420>").has_value() == false);
}

TEST(compile_time, convert_to_string_implicitly)
{
    auto constexpr result = [] () constexpr -> Result<std::string_view> {
        return "69420";
    }();

    static_assert(result.has_value() == true);
    static_assert(result.value() == "69420");
}

TEST(compile_time, convert_to_pair_implicitly)
{
    auto constexpr result = [] () -> Result<std::pair<int, std::string_view>> {
        return std::pair { 69, "420" };
    }();

    static_assert(result.has_value() == true);
    static_assert(result.value() == std::pair { 69, "420" });
}

TEST(compile_time, non_default_error_type_success)
{
    auto constexpr result = [] () -> Result<std::string_view, TraceError> {
        return "69420";
    }();

    static_assert(result.has_value() == true);
    static_assert(result.value() == "69420");
}
#endif

TEST(compile_time, return_move_only_type)
{
    auto value = [] () -> Result<std::unique_ptr<int>> {
        Result<std::unique_ptr<int>> pointer { new int(1) };
        return pointer;
    }().value();
}

TEST(compile_time, return_void_type)
{
    [[maybe_unused]] auto value = [] () -> Result<void> {
        return {};
    }().value();
}

