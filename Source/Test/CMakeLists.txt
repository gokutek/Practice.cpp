# ==============================================================================
# 公共头文件
# ==============================================================================
SET(THIRD_PARTY_ROOT_PATH ${CMAKE_HOME_DIRECTORY}/ThirdParty/)

INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH})
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/lua)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/sqlite)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/rapidxml)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/jsoncpp/include)

# ==============================================================================
# 一些单文件项目
# ==============================================================================
add_executable(test_hello_world test_hello_world.cpp)
add_executable(test_rapidxml test_rapidxml.cpp)
add_executable(test_skiplist test_skiplist.cpp)
add_executable(test_time_wheel test_time_wheel.cpp)

# ==============================================================================
# lua测试项目
# ==============================================================================
add_executable(test_lua_capi test_lua_capi.cpp)
target_link_libraries(test_lua_capi lualib)

# ==============================================================================
# lua扩展库（动态链接库）
# ==============================================================================
add_library(test_lua_ext_lib SHARED test_lua_ext_lib.cpp)
target_link_libraries(test_lua_ext_lib lualib)

# ==============================================================================
# test_simple_proto
# ==============================================================================
add_executable(test_simple_proto
    ${THIRD_PARTY_ROOT_PATH}/jsoncpp/src/lib_json/json_reader.cpp
    ${THIRD_PARTY_ROOT_PATH}/jsoncpp/src/lib_json/json_writer.cpp
    ${THIRD_PARTY_ROOT_PATH}/jsoncpp/src/lib_json/json_value.cpp
    test_simple_proto.cpp)

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

# ==============================================================================
# test_lunasvg: 渲染SVG
# ==============================================================================
add_subdirectory(../../ThirdParty/lunasvg ../../ThirdParty/lunasvg/build)
add_executable(test_lunasvg test_lunasvg.cpp)
target_link_libraries(test_lunasvg lunasvg)  
