add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

# cache support
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

# IDE support
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

# macro to add tests
macro(add_unittest test_name test_files test_lib)
    add_executable(${test_name} ${test_files})

    target_link_libraries(${test_name} PRIVATE gtest gmock gtest_main ${test_lib})

    gtest_discover_tests(${test_name}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${test_name} PROPERTIES FOLDER tests)
endmacro()

add_unittest(CalcTest CalculatorTest.cpp calc)
