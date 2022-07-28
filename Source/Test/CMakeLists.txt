# ==============================================================================
# Headers and sources
# ==============================================================================

# ==============================================================================
# Target definition
# ==============================================================================

set(TARGET_TEST_HELLO_WORLD test_hello_world)

add_executable(${TARGET_TEST_HELLO_WORLD} test_hello_world.cpp)

# ==============================================================================
# Target properties
# ==============================================================================

set_target_properties(${TARGET_TEST_HELLO_WORLD} PROPERTIES C_STANDARD 90)
set_target_properties(${TARGET_TEST_HELLO_WORLD} PROPERTIES C_EXTENSIONS OFF)
set_target_properties(${TARGET_TEST_HELLO_WORLD} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)

# ==============================================================================
# Compile options
# ==============================================================================

if(MSVC)
    target_compile_options(${TARGET_TEST_HELLO_WORLD} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(${TARGET_TEST_HELLO_WORLD} PRIVATE /fp:fast)
endif()
