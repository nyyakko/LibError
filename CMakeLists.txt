cmake_minimum_required(VERSION 3.25)

project(LibError LANGUAGES CXX)

include(cmake/get_cpm.cmake)
include(cmake/enable_tests.cmake)
include(GNUInstallDirs)

if (ENABLE_TESTING)
    enable_tests(${PROJECT_NAME})
endif()

CPMAddPackage("gh:fmtlib/fmt#10.2.1")

CPMFindPackage(
    NAME expected
    GITHUB_REPOSITORY nyyakko/expected
    GIT_TAG master
)

include(cmake/static_analyzers.cmake)

# set(LibError_CompilerOptions ${LibError_CompilerOptions})
# set(LibError_LinkerOptions ${LibError_LinkerOptions})

set(LibError_ExternalLibraries fmt::fmt tl::expected)

add_subdirectory(LibError)
