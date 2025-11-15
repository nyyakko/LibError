#pragma once

#include <string_view>
#include <variant>

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
concept error_policy_concept = requires (T error, T::error_t)
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

template <class T, error_policy_concept ErrorPolicy = error::Default>
using Result = LIBERROR_EXP::expected<std::conditional_t<std::is_void_v<T>, std::monostate, T>, ErrorPolicy>;

template <error_policy_concept ErrorPolicy = error::Default>
constexpr auto make_error(typename ErrorPolicy::error_t message, auto&&... arguments)
{
    if constexpr (sizeof...(arguments))
        return LIBERROR_EXP::unexpected<ErrorPolicy>(LIBERROR_FMT::vformat(message, LIBERROR_FMT::make_format_args(arguments...)));
    else
        return LIBERROR_EXP::unexpected<ErrorPolicy>(message);
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
    using error_t = std::string_view;

    constexpr explicit Default(std::string_view message) : message_m { message } {}

    constexpr Default() noexcept = default;
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

