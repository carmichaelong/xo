file (GLOB TEST_FILES "*.h" "*.cpp")

add_executable(xo_test ${TEST_FILES})

target_link_libraries(xo_test xo)

set_target_properties(xo_test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

set_target_properties(xo_test PROPERTIES
	PROJECT_LABEL "xo_test"
	OUTPUT_NAME xo_test
    FOLDER "xo" # Place target inside a xo folder in MSVC
	)

target_compile_definitions( xo_test PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:XO_ENABLE_PROFILER>" )
