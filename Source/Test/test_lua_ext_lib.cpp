extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

static int lencode(lua_State *L) 
{
	lua_pushnumber(L, 666);
	lua_pushstring(L, "hello");
	return 2; // ����ֵ��ʾջ���м�������ֵ
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
	// ����������
	int n = lua_gettop(L);
	if (n != 2) 
	{
		return luaL_error(L, "Invalid parameter count");
	}
	
	// -1��ʾջ��Ԫ�ص�����������lua�����е��ú���ʱд�����ұߵ�ʵ��
	lua_Number a = lua_tonumberx(L, -1, nullptr);
	lua_Number b = lua_tonumberx(L, -2, nullptr);
	lua_pushnumber(L, a + b);
	return 1;
}

/*
=======================================================================
�������������������luaopen_MyExtension_Math��������lua��������Ҫ��ôʹ�ã�
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
