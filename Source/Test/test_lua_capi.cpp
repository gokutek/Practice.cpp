#include <iostream>
#include <assert.h>
#include <string.h>
#include <windows.h>
#include <string>
#include <stdio.h>
#include <io.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static void test_lua_type()
{
	lua_State* L = luaL_newstate();

	// 虽然可以向栈中放integer和number，但它们的类型都是LUA_TNUMBER
	lua_pushinteger(L, 1);
	int type = lua_type(L, -1);
	assert(type == LUA_TNUMBER);

	lua_pushnumber(L, 1.23);
	type = lua_type(L, -1);
	assert(type == LUA_TNUMBER);

	lua_pushstring(L, "1.23");
	type = lua_type(L, -1);
	assert(type == LUA_TSTRING);

	lua_close(L);
}

static void test_lua_rotate()
{
	lua_State* L = luaL_newstate();

	/*
	==============================================================================
	Index: 1	2	3	4	5
	Stack: 10	20	30	40	50
	第二个参数表示从索引为3到栈顶之间的元素
	第三个参数1表示向栈顶方向旋转
	可以将这些元素想象成一个圆环，然后顺时针或逆时针旋转它。旋转之后为：10 20 50 30 40
	==============================================================================
	*/
	lua_pushinteger(L, 10);
	lua_pushinteger(L, 20);
	lua_pushinteger(L, 30);
	lua_pushinteger(L, 40);
	lua_pushinteger(L, 50);
	lua_rotate(L, 3, 1);
	assert(lua_tointeger(L, 1) == 10);
	assert(lua_tointeger(L, 2) == 20);
	assert(lua_tointeger(L, 3) == 50);
	assert(lua_tointeger(L, 4) == 30);
	assert(lua_tointeger(L, 5) == 40);

	/*
	==============================================================================
	Index: 1	2	3	4	5
	Stack: 10	20	30	40	50
	与上例的不同之处在于第三个参数是-1，表示向栈底方向旋转。
	旋转之后为：10 20 40 50 30
	==============================================================================
	*/
	lua_settop(L, 0);
	lua_pushinteger(L, 10);
	lua_pushinteger(L, 20);
	lua_pushinteger(L, 40);
	lua_pushinteger(L, 50);
	lua_pushinteger(L, 30);
	lua_rotate(L, 3, -1);

	lua_close(L);
}

static void test_lua_is_number()
{
	lua_State* L = luaL_newstate();

	lua_pushinteger(L, 10);
	assert(lua_isnumber(L, -1));
	assert(lua_isinteger(L, -1));

	// 这个字符串可以被转换为number，所以lua_isnumber仍然成立，奇怪的是lua_isinteger不成立
	lua_pushstring(L, "1234");
	assert(lua_isnumber(L, -1));
	assert(!lua_isinteger(L, -1));

	// 字符串不能转换为number
	lua_pushstring(L, "hello");
	assert(!lua_isnumber(L, -1));

	lua_close(L);
}

static void test_lua_tonumber()
{
	lua_State* L = luaL_newstate();

	lua_pushinteger(L, 10);
	lua_Number num = lua_tonumber(L, -1);
	assert(num == 10);

	lua_pushnumber(L, 12.34);
	num = lua_tonumber(L, -1);
	assert(num == 12.34);

	lua_pushstring(L, "4567");
	num = lua_tonumber(L, -1);
	assert(num == 4567);

	lua_pushstring(L, "67.89");
	num = lua_tonumber(L, -1);
	assert(num == 67.89);

	lua_close(L);
}

static void test_table()
{
	lua_State* L = luaL_newstate();
	
	lua_newtable(L);				// 1
	lua_pushstring(L, "key");		// 2
	lua_pushstring(L, "value");		// 3
	lua_settable(L, -3);			// 1
	assert(lua_gettop(L) == 1);

	lua_pushstring(L, "key");		// 2
	lua_gettable(L, -2);			// 2
	const char* value = lua_tostring(L, -1);
	assert(!strcmp(value, "value"));

	lua_close(L);
}

static void add_package_path(lua_State* L, const char* path)
{
	assert(path);
	lua_getglobal(L, "package");	// [-0, +1, e]
	lua_getfield(L, -1, "path");	// [-0, +1, e]
	char final_path[1024];
	sprintf_s(final_path, "%s;%s", lua_tostring(L, -1), path);
	lua_pushstring(L, final_path);	// [-0, +1, m]
	lua_setfield(L, -3, "path");	// [-1, +0, e]
	lua_pop(L, 2);
}

static void test_print()
{
	int result;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L); // 不打开标准库的话，print都调用不了
	add_package_path(L, "lua/?.lua");
	result = luaL_loadfile(L, "lua/test_capi_loadfile.lua");
	assert(result == 0);
	lua_pcall(L, 0, LUA_MULTRET, 0);
 	lua_close(L);
}

static void test_get_field()
{
	lua_State* L = luaL_newstate();
	lua_newtable(L);
	lua_pushstring(L, "world");
	lua_setfield(L, -2, "hello");	// [-1, +0, e]
	lua_getfield(L, -1, "hello");
	const char* value = lua_tostring(L, -1);
	assert(!strcmp(value, "world"));
	lua_close(L);
}

static void test_set_global()
{
	lua_State* L = luaL_newstate();
	lua_pushstring(L, "WORLD");
	lua_setglobal(L, "HELLO");	// [-1, +0, e]
	lua_getglobal(L, "HELLO"); // [-0, +1, e]
	const char* value = lua_tostring(L, -1);
	assert(!strcmp(value, "WORLD"));
	lua_close(L);
}

static void test_create_table()
{
	// lua_createtable内部有优化，实际上lua_newtable就是带有默认参数的lua_createtable
}

/** 调用lua文件中定义的全局函数 */
static void test_call_lua()
{
	int result;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L); // 不打开标准库的话，print都调用不了
	add_package_path(L, "lua/?.lua");

	result = luaL_loadfile(L, "lua/test_capi_call_lua.lua");	// [-0, +1, m]
	assert(result == 0);
	result = lua_pcall(L, 0, LUA_MULTRET, 0);	// [-(nargs + 1), +(nresults|1), –]
	assert(result == LUA_OK);
	lua_getglobal(L, "mymath");
	lua_pushinteger(L, 30); // 参数x
	lua_pushinteger(L, 12);	// 参数y
	lua_pcall(L, 2, LUA_MULTRET, 0);
	lua_Integer sum = lua_tointeger(L, -2);
	lua_Integer diff = lua_tointeger(L, -1);
	assert(sum == 42);
	assert(diff == 18);
	lua_pop(L, 2); // 将返回值出栈
	lua_close(L);
}

/** 调用require加载lua文件 */
static void test_call_require()
{
	int result;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L); // 不打开标准库的话，print都调用不了
	add_package_path(L, "lua/?.lua");

	lua_getglobal(L, "require");
	lua_pushstring(L, "test_capi_call_lua");
	result = lua_pcall(L, 1, LUA_MULTRET, 0);	// [-(nargs + 1), +(nresults|1), –]
	assert(result == LUA_OK);

	lua_getglobal(L, "mymath");
	lua_pushinteger(L, 30); // 参数x
	lua_pushinteger(L, 12);	// 参数y
	lua_pcall(L, 2, LUA_MULTRET, 0);
	lua_Integer sum = lua_tointeger(L, -2);
	lua_Integer diff = lua_tointeger(L, -1);
	assert(sum == 42);
	assert(diff == 18);
	lua_pop(L, 2); // 将返回值出栈
	lua_close(L);
}

static void test_newmetatable()
{
	lua_State* L = luaL_newstate();

	int ret = luaL_newmetatable(L, "ObjectMap");
	assert(1 == ret);
	lua_pop(L, 1);

	ret = luaL_newmetatable(L, "ObjectMap");
	assert(0 == ret);
	lua_pop(L, 1);

	lua_close(L);
}

//测试：在C中写_G，在Lua中读取
static void test_global_c_lua()
{
	int result;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L); // 不打开标准库的话，print都调用不了
	add_package_path(L, "lua/?.lua");

	lua_pushinteger(L, 123321);
	lua_setglobal(L, "TArray");

	result = luaL_loadfile(L, "lua/test_capi_global_c_lua.lua");	// [-0, +1, m]
	assert(result == 0);
	result = lua_pcall(L, 0, LUA_MULTRET, 0);	// [-(nargs + 1), +(nresults|1), –]
	assert(result == LUA_OK);

	lua_close(L);
}

static int error_func(lua_State* L)
{
	int Type = lua_type(L, -1);
	if (Type == LUA_TSTRING)
	{
		const char* ErrorString = lua_tostring(L, -1);
		luaL_traceback(L, L, ErrorString, 1);
		ErrorString = lua_tostring(L, -1);
		ErrorString;
	}
	else if (Type == LUA_TTABLE)
	{
		// multiple errors is possible
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			const char* ErrorString = lua_tostring(L, -1);
			ErrorString;
			lua_pop(L, 1);
		}
	}

	lua_pop(L, 1);

#if 1
	lua_pushinteger(L, 58661244);
	return 1;
#else
	// 若这里不push，等价于pushnil
	return 0;
#endif
}

static void test_pcall_ok()
{
	lua_State* L = luaL_newstate();
	lua_pushcfunction(L, error_func);
	luaL_loadfile(L, "lua/test_capi_pcall_ok.lua");	// [-0, +1, m]
	if (LUA_OK != lua_pcall(L, 0, LUA_MULTRET, 1))
	{
		lua_pop(L, 1);
	}

	int type = lua_getglobal(L, "test_pcall");
	assert(type == LUA_TFUNCTION);

	if (LUA_OK != lua_pcall(L, 0, LUA_MULTRET, 1))
	{
		lua_pop(L, 1);
	}

	lua_Integer value = lua_tointeger(L, -1);
	assert(value == 12345);
	lua_pop(L, 1); // 弹出 返回值

	lua_pop(L, 1); // 弹出 error_func
	
	assert(0 == lua_gettop(L));

	lua_close(L);
}

static void test_pcall_err()
{
	lua_State* L = luaL_newstate();
	lua_pushcfunction(L, error_func);
	luaL_loadfile(L, "lua/test_capi_pcall_err.lua");	// [-0, +1, m]
	if (LUA_OK != lua_pcall(L, 0, LUA_MULTRET, 1))
	{
		lua_pop(L, 1);
	}

	int type = lua_getglobal(L, "test_pcall");
	assert(type == LUA_TFUNCTION);

	if (LUA_OK != lua_pcall(L, 0, LUA_MULTRET, 1))
	{
		assert(58661244 == lua_tointeger(L, -1));
		lua_pop(L, 1);
	}

	lua_pop(L, 1); // 弹出 error_func

	assert(0 == lua_gettop(L));

 	lua_close(L);
}

static int closure001(lua_State* L)
{
	lua_Integer x = lua_tointeger(L, lua_upvalueindex(1));
	lua_Integer y = lua_tointeger(L, lua_upvalueindex(2));
	lua_Integer z = lua_tointeger(L, lua_upvalueindex(3));
	assert(x == 10);
	assert(y == 20);
	assert(z == 30);
	return 0;
}

static void test_pushcclosure()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	add_package_path(L, "lua/?.lua");

	// 先push3个upvalue
	lua_pushinteger(L, 10);
	lua_pushinteger(L, 20);
	lua_pushinteger(L, 30);
	lua_pushcclosure(L, closure001, 3);
	lua_setglobal(L, "test_closure");
	assert(lua_gettop(L) == 0);

	luaL_loadfile(L, "lua/test_capi_closure.lua");	// [-0, +1, m]
	if (LUA_OK != lua_pcall(L, 0, LUA_MULTRET, 1))
	{
		lua_pop(L, 1);
	}

	int type = lua_getglobal(L, "main");
	assert(type == LUA_TFUNCTION);
	lua_pcall(L, 0, LUA_MULTRET, 0);

	lua_close(L);
}

static void init_work_dir()
{
	char szWorkDir[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szWorkDir, sizeof(szWorkDir));
	while (true)
	{
		*strrchr(szWorkDir, '\\') = 0;

		std::string tmp = szWorkDir;
		tmp += "\\Assets\\";
		if (!access(tmp.c_str(), 0))
		{
			SetCurrentDirectoryA(tmp.c_str());
			break;
		}

	}
}

int main()
{
	init_work_dir();

	test_lua_type();
	test_lua_rotate();
	test_lua_is_number();
	test_lua_tonumber();
	test_table();
	test_print();
	test_get_field();
	test_set_global();
	test_create_table();
	test_call_lua();
	test_call_require();
	test_newmetatable();
	test_global_c_lua();
	test_pcall_ok();
	test_pcall_err();
	test_pushcclosure();

	return 0;
}

/*
===============================================================================
http://www.lua.org/manual/5.4/
API后面的[-o, +p, x]意思：
The first field, o, is how many elements the function pops from the stack.
The second field, p, is how many elements the function pushes onto the stack. 
(Any function always pushes its results after popping its arguments.) A field 
in the form x|y means the function can push (or pop) x or y elements, depending 
on the situation; an interrogation mark '?' means that we cannot know how many 
elements the function pops/pushes by looking only at its arguments. (For instance, 
they may depend on what is in the stack.) The third field, x, tells whether the 
function may raise errors: '-' means the function never raises any error; 'm' 
means the function may raise only out-of-memory errors; 'v' means the function 
may raise the errors explained in the text; 'e' means the function can run 
arbitrary Lua code, either directly or through metamethods, and therefore may 
raise any errors.
===============================================================================
*/
