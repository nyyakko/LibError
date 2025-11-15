#pragma once

#include "Result.hpp"

#if !(defined(__clang__) || defined(__GNUC__))
#error "Compiler doesn't support [compound-expressions](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)"
#endif

#define TRY(expression) ({                                                                   \
    using namespace liberror;                                                                \
    auto&& _ = (expression);                                                                 \
    if (!_.has_value()) return make_error(_.error().message());                              \
    std::move(_).value();                                                                    \
})

#define MUST(expression) ({                                                                  \
    auto&& _ = (expression);                                                                 \
    if (!_.has_value())                                                                      \
    {                                                                                        \
        LIBERROR_FMT::println(stderr, "Aborted execution because: {}", _.error().message()); \
        std::abort();                                                                        \
    }                                                                                        \
    std::move(_).value();                                                                    \
})

