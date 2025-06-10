cmake_minimum_required(VERSION 3.25)

project(LibError LANGUAGES CXX)

include(cmake/get_cpm.cmake)
include(GNUInstallDirs)

CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:nyyakko/expected#master")

include(cmake/static_analyzers.cmake)
include(cmake/enable_tests.cmake)

if (ENABLE_TESTING)
    enable_tests(${PROJECT_NAME})
endif()

# set(LibError_CompilerOptions ${LibError_CompilerOptions})
# set(LibError_LinkerOptions ${LibError_LinkerOptions})

set(LibError_ExternalLibraries fmt::fmt tl::expected)

add_subdirectory(LibError)
