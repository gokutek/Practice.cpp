--[[
测试步骤：
在Bin目录下执行：lua.exe ..\Script\TestMyExtension.lua
--]]
local MyExtension = require("test_lua_ext_lib")
local n, s = MyExtension.encode()
print(n)
print(s)

local MyExtensionMath = require("test_lua_ext_lib.Math")
print(MyExtensionMath.add(10, 20))
