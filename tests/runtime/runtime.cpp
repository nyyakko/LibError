#include <gtest/gtest.h>

#include <liberror/ErrorOr.hpp>
#include <liberror/types/TraceError.hpp>

#include <array>

using namespace liberror;

static constexpr auto BUFFER_SIZE = 1024;

struct S
{
    explicit constexpr S(std::string value) noexcept : value_m { value } { std::println("S::S(std::string)"); }
    explicit constexpr S(char const* value) noexcept : value_m { value } { std::println("S::S(char const*)"); }
    constexpr ~S() noexcept { std::println("S::~S()"); }
    constexpr S(S const& s) noexcept : value_m { s.value_m } { std::println("S::S(S const&)"); }
    constexpr S(S&& s) noexcept : value_m { std::move(s.value_m) } { std::println("S::S(S&&)"); }
    constexpr S& operator=(S const& s) { value_m = s.value_m; std::println("S::S operator=(S const&)"); return *this; }
    constexpr S& operator=(S&& s) noexcept { value_m = std::move(s.value_m); std::println("S::S operator=(S&&)"); return *this; }

    constexpr S& operator=(char const* lhs) { value_m = lhs; return *this; }

    std::string value_m {};
};

int redirect_stdout_to_buffer(std::array<char, BUFFER_SIZE>& buffer)
{
    fflush(stdout);
    auto previousState = _dup(_fileno(stdout));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    (void)freopen("NUL", "a", stdout);
#pragma clang diagnostic pop
    setvbuf(stdout, buffer.data(), _IOFBF, buffer.size());

    return previousState;
}

void restore_stdout(int state)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    (void)freopen("NUL", "a", stdout);
#pragma clang diagnostic pop
    _dup2(state, _fileno(stdout));
    setvbuf(stdout, NULL, _IONBF, BUFFER_SIZE);
}

TEST(compile_time, non_default_error_type_failure)
{
    auto result = [] () -> ErrorOr<std::string_view, TraceError> {
        return make_error<TraceError>("failure");
    }();
}

TEST(runtime, no_error)
{
    auto result = [] () -> ErrorOr<std::string_view> {
        return "69420";
    }();

    EXPECT_STREQ(result.value().data(), "69420");
}

TEST(runtime, single_error)
{
    auto result = [] () -> ErrorOr<std::string> {
        return make_error("error");
    }();

    EXPECT_STREQ(result.error().message().data(), "error");
}

TEST(runtime, multiple_error_lvalue)
{
    auto result = [] () -> ErrorOr<std::string> {
        auto error = [] () -> ErrorOr<std::string> {
            return make_error("first error {}", 69);
        }();
        if (error.has_error()) return make_error(error.error());
        return make_error("second error");
    }();

    EXPECT_STREQ(result.error().message().data(), "first error 69");
}

TEST(runtime, multiple_error_rvalue)
{
    auto result = [] () -> ErrorOr<std::string> {
        auto fnMakeError = [] () -> ErrorOr<std::string> {
            return make_error("first error {}", 69);
        };
        return make_error(fnMakeError().error());
    }();

    EXPECT_STREQ(result.error().message().data(), "first error 69");
}

TEST(runtime, assignment_operator)
{
    std::array<char, BUFFER_SIZE> buffer {};
    auto const previousState = redirect_stdout_to_buffer(buffer);
    {
        auto value = [] () -> ErrorOr<S> {
            using namespace std::literals;
            ErrorOr<S> value { ""s };
            return value;
        }();
        value = "hello";
    }
    restore_stdout(previousState);
    EXPECT_STREQ(buffer.data(), "S::S(std::string)\nS::~S()\n");
}

