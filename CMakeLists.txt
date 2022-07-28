cmake_minimum_required(VERSION 3.10)
project(Practice.cpp)

# ==============================================================================
# Headers and sources
# ==============================================================================

set(HEADERS
)

set(SOURCES
    Source/Test/test_hello_world.cpp
)

# ==============================================================================
# Target definition
# ==============================================================================

set(TARGET test_hello_world)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

# ==============================================================================
# Target properties
# ==============================================================================

set_target_properties(${TARGET} PROPERTIES C_STANDARD 90)
set_target_properties(${TARGET} PROPERTIES C_EXTENSIONS OFF)
set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)

# ==============================================================================
# Compile options
# ==============================================================================

if(MSVC)
    target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(${TARGET} PRIVATE /fp:fast)
endif()

# ==============================================================================
# Link libraries
# ==============================================================================

if(WIN32)
    # nothing to do for now
elseif(APPLE)
    target_link_libraries(${TARGET} PRIVATE "-framework Cocoa")
endif()

# ==============================================================================
# IDE support
# ==============================================================================

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})
