set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LibError_HeaderFiles ${LibError_HeaderFiles}
    "${DIR}/Result.hpp"
    "${DIR}/Try.hpp"

    PARENT_SCOPE
)
