# ==============================================================================
# 公共头文件
# ==============================================================================
SET(THIRD_PARTY_ROOT_PATH ${CMAKE_HOME_DIRECTORY}/third_party/)

INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH})
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/lua)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/sqlite)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/rapidxml)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/jsoncpp/include)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/ratas/src)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/sigslot/include)

# ==============================================================================
# 一些单文件项目
# ==============================================================================
function(add_single_file_project fname)
    add_executable(${fname} ${fname}.cpp)
    set_property(TARGET ${fname} PROPERTY FOLDER "test")
endfunction()

add_single_file_project(test_hello_world)
add_single_file_project(test_rapidxml)
add_single_file_project(test_skiplist)
add_single_file_project(test_time_wheel)
add_single_file_project(test_stl_algo)
add_single_file_project(test_monte_carlo)
add_single_file_project(test_ratas)
add_single_file_project(test_filesystem)
add_single_file_project(test_openssl)
add_single_file_project(test_dtor)
add_single_file_project(test_misc)
add_single_file_project(test_asm)
add_single_file_project(test_win_iocp)
add_single_file_project(test_ring_buffer)
add_single_file_project(test_udp_socket)
add_single_file_project(test_variadic_template)
add_single_file_project(test_printf_format)
add_single_file_project(test_tagged_pointer)

# ==============================================================================
# lua测试项目
# ==============================================================================
add_executable(test_lua_capi test_lua_capi.cpp)
target_link_libraries(test_lua_capi lualib)
set_property(TARGET test_lua_capi PROPERTY FOLDER "lua")

# ==============================================================================
# lua扩展库，动态链接库，必须加上LUA_BUILD_AS_DLL、LUA_LIB以导出符号
# ==============================================================================
add_library(test_lua_ext_lib SHARED test_lua_ext_lib.cpp)
target_link_libraries(test_lua_ext_lib lualib)
target_compile_definitions(test_lua_ext_lib PRIVATE LUA_BUILD_AS_DLL PRIVATE LUA_LIB)
set_property(TARGET test_lua_ext_lib PROPERTY FOLDER "lua")

# ==============================================================================
# test_simple_proto
# ==============================================================================
add_executable(test_simple_proto
    ${THIRD_PARTY_ROOT_PATH}/jsoncpp/src/lib_json/json_reader.cpp
    ${THIRD_PARTY_ROOT_PATH}/jsoncpp/src/lib_json/json_writer.cpp
    ${THIRD_PARTY_ROOT_PATH}/jsoncpp/src/lib_json/json_value.cpp
    test_simple_proto.cpp)
set_property(TARGET test_simple_proto PROPERTY FOLDER "test")

# ==============================================================================
# test_sqlite
# ==============================================================================
add_executable(test_sqlite 
    ${THIRD_PARTY_ROOT_PATH}/sqlite/sqlite3.c
    ${THIRD_PARTY_ROOT_PATH}/microprofile/microprofile.cpp
    test_sqlite.cpp
    )
target_link_libraries(test_sqlite ws2_32)
set_property(TARGET test_sqlite PROPERTY FOLDER "test")

# ==============================================================================
# test_lunasvg: 渲染SVG
# ==============================================================================
add_subdirectory(../../third_party/lunasvg ../../third_party/lunasvg/build)
add_executable(test_lunasvg test_lunasvg.cpp)
target_link_libraries(test_lunasvg lunasvg)  
set_property(TARGET test_lunasvg PROPERTY FOLDER "test")
set_property(TARGET lunasvg PROPERTY FOLDER "third_party")

# ==============================================================================
# test_kcp
# ==============================================================================
add_executable(test_kcp 
    ${THIRD_PARTY_ROOT_PATH}/kcp/ikcp.h
    ${THIRD_PARTY_ROOT_PATH}/kcp/ikcp.c
    test_kcp.cpp
    )
target_link_libraries(test_kcp ws2_32)
set_property(TARGET test_kcp PROPERTY FOLDER "test")
