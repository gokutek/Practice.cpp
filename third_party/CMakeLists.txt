# lua静态库
add_library(lualib STATIC 
    lua/lzio.c
    lua/lctype.c
    lua/lopcodes.c
    lua/lmem.c
    lua/lundump.c
    lua/ldump.c
    lua/lstate.c
    lua/lgc.c
    lua/llex.c
    lua/lcode.c
    lua/lparser.c
    lua/ldebug.c
    lua/lfunc.c
    lua/lobject.c
    lua/ltm.c
    lua/lstring.c
    lua/ltable.c
    lua/ldo.c
    lua/lvm.c
    lua/lapi.c
    lua/lauxlib.c
    lua/lbaselib.c
    lua/lcorolib.c
    lua/ldblib.c
    lua/liolib.c
    lua/lmathlib.c
    lua/loadlib.c
    lua/loslib.c
    lua/lstrlib.c
    lua/ltablib.c
    lua/lutf8lib.c
    lua/linit.c)

# lua交互式解释器
add_executable(lua lua/lua.c)
target_link_libraries(lua lualib)
