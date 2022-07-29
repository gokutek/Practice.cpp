# ==============================================================================
# 公共头文件
# ==============================================================================
SET(THIRD_PARTY_ROOT_PATH ${CMAKE_HOME_DIRECTORY}/ThirdParty/)

INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH})
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/sqlite)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/Catch)

# ==============================================================================
# test_hello_world
# ==============================================================================
add_executable(test_hello_world test_hello_world.cpp)

# ==============================================================================
# test_skiplist
# ==============================================================================
add_executable(test_skiplist test_skiplist.cpp)

# ==============================================================================
# test_sqlite
# ==============================================================================
set(TARGET test_sqlite)

set(TARGET_SRC
    ${THIRD_PARTY_ROOT_PATH}/sqlite/sqlite3.c
    ${THIRD_PARTY_ROOT_PATH}/microprofile/microprofile.cpp
    test_sqlite.cpp)

add_executable(${TARGET} ${TARGET_SRC})
target_link_libraries(${TARGET} ws2_32)
