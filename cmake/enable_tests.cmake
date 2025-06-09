function(enable_tests PROJECT)
    if (NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        message(STATUS "[${PROJECT}] running on ${CMAKE_HOST_SYSTEM_NAME}, sanitizers for tests are enabled.")
        set(LibError_TestsLinkerOptions ${LibError_TestsLinkerOptions} -fsanitize=undefined,leak,address)
    else()
        message(STATUS "[${PROJECT}] running on ${CMAKE_HOST_SYSTEM_NAME}, sanitizers for tests are disabled.")
    endif()

    set(LibError_TestsCompilerOptions ${LibError_TestsCompilerOptions} ${LibError_CompilerOptions})
    set(LibError_TestsLinkerOptions ${LibError_TestsLinkerOptions} ${LibError_LinkerOptions})

    include(GoogleTest)

    CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG release-1.12.1
        VERSION 1.12.1
        OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
    )

    enable_testing()
    add_subdirectory(tests)
endfunction()
