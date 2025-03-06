#pragma once

#include <string_view>

#ifndef __cpp_lib_print
#include <fmt/format.h>
#define LIBERROR_FMT fmt
#else
#include <format>
#include <print>
#define LIBERROR_FMT std
#endif

#ifndef __cpp_lib_expected
#include <tl/expected.hpp>
#define LIBERROR_EXP tl
#else
#include <expected>
#define LIBERROR_EXP std
#endif

namespace liberror {

namespace error {

class [[nodiscard]] Default;

}

template <class T>
concept error_policy_concept = requires (T error)
{
    std::is_constructible_v<T, std::string_view>;
    std::is_move_constructible_v<T>;
    std::is_move_assignable_v<T>;
    std::is_copy_constructible_v<T>;
    std::is_copy_assignable_v<T>;
    std::is_nothrow_constructible_v<T>;
    std::is_nothrow_move_constructible_v<T>;
    std::is_nothrow_move_assignable_v<T>;
    error.message();
};

struct Void {};

template <class T, error_policy_concept ErrorPolicy = error::Default>
using Result = LIBERROR_EXP::expected<std::conditional_t<std::is_void_v<T>, Void, T>, ErrorPolicy>;

template <error_policy_concept ErrorPolicy = error::Default>
constexpr auto make_error(std::string_view message)
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(message);
}

template <error_policy_concept ErrorPolicy = error::Default>
constexpr auto make_error(std::string_view format, auto&&... arguments)
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(LIBERROR_FMT::vformat(format, LIBERROR_FMT::make_format_args(arguments...)));
}

template <error_policy_concept ErrorPolicy = error::Default>
constexpr auto make_error(ErrorPolicy&& error) noexcept
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(std::forward<ErrorPolicy>(error));
}

template <error_policy_concept ErrorPolicy = error::Default>
constexpr auto make_error(ErrorPolicy& error) noexcept
{
    return LIBERROR_EXP::unexpected<ErrorPolicy>(std::move(error));
}

class [[nodiscard]] error::Default
{
public:
    constexpr explicit Default(std::string_view message) : message_m { message } {}

    constexpr  Default() noexcept = default;
    constexpr ~Default() noexcept = default;

    constexpr Default(Default const& error) : message_m { error.message_m } {}
    constexpr Default(Default&& error) noexcept : message_m { std::move(error.message_m) } {}

    constexpr Default& operator=(Default&& error) noexcept
    {
        message_m = std::move(error.message_m);
        return *this;
    }

    constexpr Default& operator=(Default const& error)
    {
        message_m = error.message_m;
        return *this;
    }

    [[nodiscard]] constexpr auto const& message() const noexcept { return message_m; }

private:
    std::string message_m;
};

} // liberror

