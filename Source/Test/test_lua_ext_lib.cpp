extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

static int lencode(lua_State *L) 
{
	lua_pushnumber(L, 666);
	lua_pushstring(L, "hello");
	return 2; // 返回值表示栈里有几个返回值
}

extern "C" LUAMOD_API int luaopen_MyExtension(lua_State *L) 
{
	luaL_checkversion(L);

	luaL_Reg l[] = {
		{ "encode", lencode },
		{ NULL,  NULL },
	};

	luaL_newlib(L, l);

	return 1;
}

/*
===========================================
local MyExtension = require("MyExtension")
local n, s = MyExtension.encode()
print(n)
print(s)
===========================================
*/
static int ladd(lua_State *L)
{
	// 检查参数个数
	int n = lua_gettop(L);
	if (n != 2) 
	{
		return luaL_error(L, "Invalid parameter count");
	}
	
	// -1表示栈顶元素的索引，即在lua代码中调用函数时写在最右边的实参
	lua_Number a = lua_tonumberx(L, -1, nullptr);
	lua_Number b = lua_tonumberx(L, -2, nullptr);
	lua_pushnumber(L, a + b);
	return 1;
}

/*
=======================================================================
这个导出函数的名字是luaopen_MyExtension_Math，所以在lua代码中需要这么使用：
local MyExtensionMath = require("MyExtension.Math")
print(MyExtensionMath.add(10, 20))
=======================================================================
*/
extern "C" LUAMOD_API int luaopen_test_lua_ex_lib_Math(lua_State *L)
{
	luaL_checkversion(L);

	luaL_Reg l[] = {
		{ "add", ladd },
		{ NULL,  NULL },
	};

	luaL_newlib(L, l);

	return 1;
}
