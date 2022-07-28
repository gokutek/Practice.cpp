# ==============================================================================
# 公共头文件
# ==============================================================================
SET(THIRD_PARTY_ROOT_PATH ${CMAKE_HOME_DIRECTORY}/ThirdParty/)

INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH})
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/sqlite)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# expected behaviour
#set(CMAKE_CXX_FLAGS"${CMAKE_CXX_FLAGS} /std:c++lastest")

# ==============================================================================
# test_hello_world
# ==============================================================================
set(TARGET_TEST_HELLO_WORLD test_hello_world)
add_executable(${TARGET_TEST_HELLO_WORLD} test_hello_world.cpp)

# ==============================================================================
# test_sqlite
# ==============================================================================
set(TARGET_TEST_SQLITE test_sqlite)

set(TARGET_TEST_SQLITE_SRC
    ${THIRD_PARTY_ROOT_PATH}/sqlite/sqlite3.c
    ${THIRD_PARTY_ROOT_PATH}/microprofile/microprofile.cpp
    test_sqlite.cpp)

add_executable(${TARGET_TEST_SQLITE} ${TARGET_TEST_SQLITE_SRC})
target_link_libraries(${TARGET_TEST_SQLITE} ws2_32)
