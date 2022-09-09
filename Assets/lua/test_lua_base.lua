--[[
    测试Lua语言本身的特性
--]]

package.path = package.path .. ";../Script/?.lua"

--判断数组是否已排序（升序）
local function is_table_sorted(t)
	for i=2,#t do
		if t[i] < t[i-1] then
			return false
		end
	end
	return true
end

local function table_len(t)
	local len = 0
	for k,v in pairs(t) do
		len = len + 1
	end
	return len
end

--测试内容：删除表元素（这里测试删除最后一个元素仅仅是为了避免`#`语义的问题）
local function test_table_del_element()
	local t = { 1, 2, 3, 4, 5 }
	assert(#t == 5)
	t[5] = nil
	assert(#t == 4)
end

--测试内容：将浮点数作为key时，如果其小数部分为0，其将被转换为整数
local function test_table_float_key()
	local m = {}
	m[2] = "2"
	m[2.1] = "2.1"
	assert(m[2] == "2")
	assert(m[2.0] == "2")
	assert(m[2.1] == "2.1")
end

-- 测试内容：#操作符（The Length Operator）
local function test_table_len()
	-- 没有空洞的数组
	local t = { 1, 2, 3 }
	assert(#t == 3)

	-- 注意，下例中返回4，而不是2
	local a = {}
	a[1] = 1
	a[2] = 2
	a[3] = nil
	a[4] = 4
	assert(#a == 4)
end

-- 测试内容：table.insert (list, [pos,] value)
local function test_table_insert()
	--pos默认是#list+1
    local list = {}
	table.insert(list, 10)
	assert(list[#list] == 10)
	table.insert(list, 20)
	assert(list[#list] == 20)
	table.insert(list, 30)
	assert(list[#list] == 30)
	--[[
	--下面的代码将抛出异常：bad argument #2 to 'insert' (position out of bounds)
	list = {}
	list[1] = 10
	list[5] = 50
	table.insert(list, 6, 60)
	--]]
end

-- 测试string
local function test_string_gmatch()
    local str = "123 456"
    for k,v in str:gmatch("(%d+)") do
        print(k)
    end
end

-- 打印加载的模块
local function test_print_loaded_package()
    for k,v in pairs(package.loaded) do
        print(k)
    end
end

-- 测试table的一种初始化
local function test_table_init2()
    ROLE_TYPE = {
        HERO = 1,
        MONSTER = 2,
        PLAYER = 3,
    }
    
    -- 必须要在ROLE_TYPE.HERO两边加上[]，否则编译不过
    m = {
        [ROLE_TYPE.HERO] = 3,
        [ROLE_TYPE.MONSTER] = 4,
        [ROLE_TYPE.PLAYER] = 5,
    }

    assert(m[1] == 3)
	assert(m[2] == 4)
	assert(m[3] == 5)
end

-- -- 获取table长度
-- local function test_table_len()
-- 	gdEquipEnchantPromote = {}
	
-- 	gdEquipEnchantPromote[1] = {}
-- 	gdEquipEnchantPromote[2] = {}
-- 	gdEquipEnchantPromote[3] = {}
-- 	gdEquipEnchantPromote[4] = {}
-- 	gdEquipEnchantPromote[15] = {}
	
-- 	--[[
-- 	table.getn已经废弃了
-- 	assert(4 == table.getn(gdEquipEnchantPromote))
-- 	print("gdEquipEnchantPromote lengths is: " .. tostring(table.getn(gdEquipEnchantPromote)))
-- 	--]]
	
-- 	assert(4 == #gdEquipEnchantPromote)
-- 	print("gdEquipEnchantPromote lengths is: " .. tostring(#gdEquipEnchantPromote))
-- end

-- 测试变量作用域
local function test_var_scope()
	local val = 100
	
	if true then
		local val = 90
		val = val + 1
		assert(val == 91)
	end
	
	assert(val == 100)
end

--测试正则匹配
local function test_gmatch()
    local desc = "被攻击时,有[g5%]的几率触发[r不破金刚]：物理、魔法免伤提高：[g10%], 每[g60]秒最多触发1次"
	
    for s in string.gmatch(desc, "%[.*%]") do
        print(s)
    end

    --懒惰匹配
    -- for s in string.gmatch(desc, "%[.*?%]") do
    --     print(s)
    -- end
	
	--匹配出中括号[]中的所有文本
	for _, _, s, _ in string.gmatch(desc, "()%[(%a)(.-)%]()") do
        print(s)
    end
end

--模拟“三目运算”
local function test_threeop()
	assert((1 > 3 and 1 or 0) == 0)
	assert((1 > 3 and 0 or 1) == 1)
	assert((3 > 1 and 0 or 1) == 0)
	assert((3 > 1 and nil or 1) == 1)
	--[[
	https://www.runoob.com/w3cnote/trinocular-operators-in-lua.html
	运算符优先级从高到低为：not、and、or
	x and y or z：
	x and y为true：则返回y
	x and y为false：则返回z
	综上所述：
	--]]
end

-- 测试nil作为key
local function test_nil_key()
	local tab = { 1, 2, 3 }
	assert(tab[1] == 1)
	assert(tab[nil] == nil)
	
	--[[运行时将报错
	tab[nil] = 666
	--]]
end

--测试函参个数
local function test_func_param_count()
	local sum = function(x, y, z)
		local res = 0
		res = res + (x or 0)
		res = res + (y or 0)
		res = res + (z or 0)
		return res
	end
	--少传的参数为nil，多传的参数将被忽略
	assert(sum(1) == 1)
	assert(sum(1, 2) == 3)
	assert(sum(1, 2, 3) == 6)
	assert(sum(1, 2, 3, 4) == 6)
end

local function test_gfind1()
	--gfind也废弃了，使用gmatch
	local reward = "1,30021"
	for k, v in string.gmatch(reward, "(%d+)") do
		print(k)
	end
end

--字符串匹配模式（捕获）
local function test_patterns_1()
	string.gsub("x123abc123abc123f", "()123()", print)
	string.gsub("x123abc123abc123f", "()%d()", print)
	string.gsub("x123abc123abc123f", "%d", print)
	string.gsub("x123abc123abc123f", "(%d)", print)
	
	print(string.find("hello", "world"))
end

--测试函数的多个返回值
local function test_return_multi_value()
	local func = function(x,y)
		return x+y,x-y,x*y,x/y
	end
	
	local x,y,z,w = func(6,3)
	assert(x == 9)
	assert(y == 3)
	assert(z == 18)
	assert(w == 2)

	--加两层括号，将只打印第一个返回值
	local x1,y1,z1,w1 = (func(6,3))
	assert(x1 == 9)
	assert(y1 == nil)
	assert(z1 == nil)
	assert(w1 == nil)
end

--测试全局变量
local function test_global_val()
	do
		--下面的abc将定义在全局
		abc = 123456
	end
	
	assert(abc == 123456)
end

--测试全局变量
local function test_global_val1()
	--这里可以读到上面函数中定义的abc
	assert(abc == 123456)
end

--测试#取长度
local function test_sharp()
	local tab = { 1, 2, 3}
	assert(#tab == 3, "len is:" .. #tab)
	
	--加了一个键值，取到的长度并不会增加
	tab["key1"] = 10
	assert(#tab == 3, "len is:" .. #tab)
end

--测试调用有且只有一个参数的函数，不带()
local function test_func_call_without_bracket()
	local f1 = function(str)
		return str
	end

	local x = f1 "helloworld"
	assert(x == "helloworld")
	
	--[[
	如果有2个或0个参数，必须要加()
	local f2 = function(str1, str2)
		print(str1 .. "," .. str2)
	end
	f2("helloworld", "fuck")
	
	local f0 = function()
		print("what?")
	end
	f0()
	--]]
end

--定义一个临时数组并用下标访问
local function test_def_and_use_array()
	--注意要加上()
	local val = ({1,2,3,4,5})[3]
	assert(val == 3)
end

local function test_cmp11()	
	--虽然tom和jim两个对象的内容确实是相等的，但是比较的只是引用（指针）本身
	local tom = { age=18 }
	local jim = { age=18 }
	assert(tom ~= jim)
	
	--table.insert插入的也是对象引用，而不会将对象拷贝一份
	local tab = {}
	table.insert(tab, tom)
	tom.age = 20
	assert(tab[1].age == 20)
end

--os.date
local function test_os_date()
	--不带参数时，返回一个字符串
	local date_str = os.date()
	assert(type(date_str) == "string")
	
	--带参数，返回一个table
	local tab = os.date("*t")
	assert(type(tab.year) == "number")
	assert(type(tab.month) == "number")
	assert(type(tab.day) == "number")
	assert(type(tab.hour) == "number")
	assert(type(tab.min) == "number")
	assert(type(tab.sec) == "number")
	assert(type(tab.yday) == "number")
	assert(type(tab.wday) == "number")
	assert(type(tab.isdst) == "boolean")
		
	--!表示世界时间
	local world_date = os.date("!*t")
	assert(world_date.hour == tab.hour - 8)
end

--os.time
local function test_os_time()
	--不带参数，返回的是代表当前时间的秒数
	local now = os.time()
	assert(type(now) == "number")
	
	--带参数，返回的是该时刻的秒数
	local date = os.date("*t")
	assert(type(date) == "table")
	date.sec = date.sec - 10
	local tm = os.time(date)
	assert(tm == now - 10)
end

--loadstring
local function test_loadstring()
	--TODO:
end

local function test_bug01()
	--在lua中，将table的某一个value赋值nil，在table中是不会存入这个key值的
	local arr = { nil, nil, nil, nil }
	assert(#arr == 0)
end

--ipairs与pairs：https://www.runoob.com/note/11315
local function test_ipairs_pairs()
	local tab =
	{
		[1] = 10,
		[2] = 20,
		[3] = 30,
		[10] = 100,
	}
	
	--使用ipairs，不会迭代到10这个键
	local keys = {}
	for k,v in ipairs(tab) do
		table.insert(keys, k)
	end
	assert(#keys==3)
	
	keys = {}
	for k,v in pairs(tab) do
		table.insert(keys, k)
	end
	assert(#keys==4)
end

--测试：计算还有多少秒到0点
local function test_time_to_0()
	--一天总共多少秒
	local total_seconds = 24 * 60 * 60 
	
	--今天的开始时间（秒）
	local date = os.date("*t")
	date.hour = 0
	date.min = 0
	date.sec = 0
	local start = os.time(date)
	
	--当前时间（秒）
	local now = os.time()
	
	--今天已经过了的时间（秒）
	local passed = os.difftime (now, start)
	
	--剩余时间
	local ret = total_seconds - passed
end

-- 测试内容：do ... end块的作用域
local function test_do_end_block()
	local x = 10
	
	do
		local x = 100
		x = 99
		assert(x == 99)
	end
	
	assert(x == 10)
end

-- 测试内容：for语句中的初始变量的作用域
local function test_for_i()
	local i = 100
	for i=1,3 do
	end
	assert(i == 100)
end

-- 测试内容：goto的用法
local function test_goto()
	--[[下面的代码运行将报错
	goto continue
	local age = 18
	::continue::
	print(age)
	--]]
end

-- 测试内容：闭包
local function test_closure()

	--调用foo函数将创建一个闭包，闭包里访问了外层变量mt，如果外层变量是引用类型，则闭包也包含该引用
	local foo = function(mt)
		return function() return #mt end
	end
	
	local arr = {1,2,3}
	local func1 = foo(arr)
	local func2 = foo(arr)
	
	assert(func1() == 3)
	assert(func2() == 3)
	
	arr[4] = 4
	assert(func1() == 4)
	assert(func2() == 4)
end

local function test_require()
--[[
	require("./MyLib")
	require("MyLib")
	require("MyLib")
	require("./MyLib")
--]]
end

--测试：load函数
local function test_load()
	local func = load("return 666")
	assert(666 == func())
	
	local x = 100
	local val = load("return " .. x)()
	assert(val == 100)
end

--测试：key顺序
local function test_pairs_order()
	local t = {
		[927038]= {cost_count=0,cost_honour=20000,cost_sid=0,sid=927038,},
		[927039]= {cost_count=0,cost_honour=15000,cost_sid=0,sid=927039,},
		[927040]= {cost_count=0,cost_honour=5000,cost_sid=0,sid=927040,},
		[927041]= {cost_count=0,cost_honour=5000,cost_sid=0,sid=927041,},
		[927042]= {cost_count=0,cost_honour=5000,cost_sid=0,sid=927042,},
		[927043]= {cost_count=0,cost_honour=5000,cost_sid=0,sid=927043,},
		[927044]= {cost_count=0,cost_honour=5000,cost_sid=0,sid=927044,},
		[927045]= {cost_count=0,cost_honour=5000,cost_sid=0,sid=927045,},
	}
	
	--使用pairs遍历table,并将key保存下来
	local keys = {}
	for sid, sd in pairs(t) do
		keys[#keys+1] = sid
	end
	
	--遍历顺序并不是定义t时的顺序，也不是升序
	assert(not is_table_sorted(keys))
	
	table.sort(keys, function(a,b) return a < b end)
	assert(is_table_sorted(keys))
end

--测试：table排序
local function test_table_sort2()
	local array = {
		{ ZOrder=0, 	name="btn1", },
		{ ZOrder=100,	name="btn2", },
		{ ZOrder=0, 	name="btn3", },
		{ ZOrder=2, 	name="btn4", },
		{ ZOrder=1, 	name="btn5", },
		{ ZOrder=0, 	name="btn6", },
	}

	table.sort(array, function(a, b)
			-- print("--cmp:"..a.name..","..b.name)
			return a.ZOrder < b.ZOrder 
		end)

	--相同的ZOrder排序后仍然保留它们排序前的顺序
	assert(array[1].name == "btn1")
	assert(array[2].name == "btn3")
	assert(array[3].name == "btn6")
end

--测试：_ENV
local function test_ENV()
	-- _ENV = nil
	-- print("this is error")
	-- itIsError = 10
	assert(_ENV == _G)
end

--测试：只有一个字符串参数的函数，调用时省略括号
local function test_single_str_param()
	local function func1(str)
		return str
	end
	
	assert(func1("hello") == "hello")
	assert((func1 "hello")  == "hello")

	--func1 123 -- 编译失败，数值类型必须加括号
end

local function test_class()
	local Account = { balance = 0 }

	function Account:new(obj)
		obj = obj or {}
		self.__index = self
		setmetatable(obj, self)
		return obj
	end

	function Account:deposit(v)
		self.balance = self.balance + v
	end

	local account = Account:new()
	assert(account.balance == 0)
	account:deposit(100)
	assert(account.balance == 100)
end

--测试：`__index`
local function test_index()
	local indexCalledCount = 0

	local tab = {}
	local mt = {}
	setmetatable(tab, mt)
	mt.__index = function(t, k)
		indexCalledCount = indexCalledCount + 1
		return rawget(t, k)
	end

	--tab中不存在name字段，会触发__index元方法调用
	local v = tab.name
	assert(indexCalledCount == 1)

	--tab中不存在name字段，会触发__index元方法调用
	v = tab.name
	assert(indexCalledCount == 2)
	
	tab.name = "lazy"
	
	--tab中存在name字段，不会触发__index元方法调用
	v = tab.name
	assert(indexCalledCount == 2)
end

--测试：递归的`__index`
local function test_rec_index()
	-- local a = {}
	-- local b = {}
	-- local c = {}
	-- local d = {}
	
	-- setmetatable(a, b)
	-- setmetatable(b, c)
	-- setmetatable(c, d)
	-- b.__index = c
	-- c.__index = d

	-- --
	-- d.age = 18
	-- assert(a.age == 18)

	-- a.age = a.age + 10
	-- assert(a.age == 28)
	-- assert(d.age == 18)
end

local function test_str_int_key()
	-- "6"和6是不同的key
	local t = {}
	t["6"] = 666
	assert(t[6] == nil)
end

local function test_coroutine_hello()
	local function func(x, y, z)
		assert(x == 10)
		for i=1,10 do
			print("test_coroutine_hello:"..i)
			print(coroutine.yield())
		end
	
		table.insert(fff,abf) --为毛不会报错？
	end

	local co = coroutine.create(func)

	print(coroutine.resume(co, 1, 2, 3))

	for i=1,11 do
		print(coroutine.resume(co, i *10))
	end
end

--协程的方式实现迭代器
local function test_coroutine_iter1()
	local function iter(arr)
		--协程函数
		local function cf(arr_)
			for i=1,#arr_ do
				coroutine.yield(arr_[i])
			end
		end

		--创建协程
		local co = coroutine.create(cf)

		local function iterfunc()
			--每次迭代只需要resume协程
			local ok, v = coroutine.resume(co, arr)
			assert(ok)
			return v
		end

		return iterfunc
	end

	local t = {10,20,30}
	local index = 1
	for i in iter(t) do
		assert(i == t[index])
		index = index + 1
	end
end

--简单的迭代器
local function test_iter1()
	--迭代器
	local function myiter(t)
		local index = 0
		local function f()
			index = index + 1
			if t[index] then
				return t[index] * 10
			else
				return nil
			end
		end

		return f
	end

	local mt = { 1, 2, 3, 4, 5 }
	local chkindex = 1

	for i in myiter(mt) do
		assert(i == mt[chkindex]*10)
		chkindex = chkindex + 1
	end
end

--泛型for
local function test_iter2()
	--迭代器
	local function fac(t)
		local function f(state, cv)
			cv = cv + 1
			return t[cv]
		end
		return f,{},0
	end

	local mt = { 1, 2, 3, 4, 5 }
	local chkindex = 1

	for i in fac(mt) do
		assert(i == mt[chkindex])
		chkindex = chkindex + 1
	end
end

local function test_cl()
	local function f(cv)
		cv = cv + 1
	end

	local cv = 10
	f(cv) -- 值传递
	assert(cv == 10)
end

-- __call元方法
local function test___call()
	local TArray = {}
	TArray.__call = function(f, x, y, z)
		assert(x == 1)
		assert(y == 2)
		assert(z == 3)
	end

	setmetatable(TArray, TArray)

	TArray(1,2,3)
end

local function test_int_str_key()
	local t = {}
	t[123456] = 100
	assert(t[123456]  == 100)
	assert(t["123456"]  == nil)
end

local function test_weaktable_k()
	a = {}
	setmetatable(a, {__mode="k"})

	k = {}
	a[k] = 1

	k= {}
	a[k] = 2

	assert(table_len(a) == 2)

	--强制进行垃圾回收
	collectgarbage()

	assert(table_len(a) == 1)
end

local function test_weaktable_v()
	a = {}
	setmetatable(a, {__mode="v"})

	a[1] = {}
	a[2] = {}

	assert(table_len(a) == 2)

	--强制进行垃圾回收
	collectgarbage()

	assert(table_len(a) == 0)
end

local function test_string_find()
	local str = "金属地基"
	assert(string.find(str, "哈哈") == nil)
end

---测试for循环的步长
local function test_for_step()
	for i=1,10,2 do
		assert(i%2 == 1)
	end
end

---测试number的具体类型：float、int32、int64
local function test_number_type()
	local f = 12.34
	local i32 = 66788
	local i64 = 6500000000

	assert("number" == type(f))
	assert("number" == type(i32))
	assert("number" == type(i64))

	local function isfloat(v)
		return math.floor(v) ~= v
	end

	local function isint64(v)
		return v > 0x7fffffff or v < -0x80000000
	end

	assert(isfloat(f))
	assert(not isfloat(i32))
	assert(not isfloat(i64))
	assert(not isint64(i32))
	assert(isint64(i64))
end

---元表的__index
local function test_mt_index_001()
	local mod = {}
	mod.val1 = 6667

	local ins = {}
	setmetatable(ins, {__index=mod,__newindex=function(t,k,v) rawset(t,k,v) end})

	--ins上没有val1，访问mod.val1
	assert(ins.val1 == 6667)
	
	--将触发__newindex元方法
	ins.val1 = ins.val1 + 1
	assert(ins.val1 == 6668)
	
	--元表的这个值不会变
	assert(mod.val1 == 6667)

	--比较绕人的地方是：从一个地方读，但是写却写到另外一个地方
end

local function test_tab_key_str()
	local UI_MAIN = "UI_MAIN"
	local uis = {}
	uis[UI_MAIN] = 1
	uis["UI_MAIN"] = 10
	assert(uis[UI_MAIN] == 10)
end

local function test_weak_key()
	---获取userdata关联的table
	---@param ud userdata
	---@return table
	local function getUserDataTable(ud)
		if not GUserDataMap then
			GUserDataMap = {}
			setmetatable(GUserDataMap, {__mode="k"})
		end
		GUserDataMap[ud] = GUserDataMap[ud] or {}
		return GUserDataMap[ud]
	end

	local ud = getUserDataTable(1)
	ud.configID = 1700001
	
	collectgarbage()
	
	--number作为key不会被回收?
	--TODO:其实我想知道light userdata作为key是什么情况。

	local ud = getUserDataTable(1)
	assert(ud.configID == 1700001)
end

local function test_nil_and_false()
	assert(nil ~= false)
end

local function test_ceil()
	print(math.ceil(0/32))
	print(math.ceil(1/32))
	print(math.ceil(32/32))
	print(math.ceil(33/32))
end

local function test_tab_xfsdf3()
	local bag = {}
	bag[1] = 10
	bag[5] = 50
	for i=1,#bag do
		print(bag[i])
	end
end

local function test_unpack_args()
	local function f1(...)
		local args = {...}
		for i=1,#args do
			assert(args[i] == i*10)
		end
	end

	local function f2(...)
		f1(...)
	end

	f2(10,20,30)
end

local function test_unpack_args2()
	local function f1(...)
		local args = {...}
		for i=1,#args do
			print(args[i])
			-- assert(args[i] == i*10, args[i])
		end
	end

	local function f2(...)
		f1(table.unpack({...,40,50})) --10,40,50
	end

	f2(10,20,30)
end

local function test_unpack_args3()
	local function f1(...)
		local args = {...}
		for i=1,#args do
			assert(args[i] == i*10, args[i])
		end
	end

	local function f2(...)
		local args = {...}
		table.insert(args, 40)
		table.insert(args, 50)
		f1(table.unpack(args)) --10,20,30,40,50
	end

	f2(10,20,30)
end

local function test_unpack_args4()
	local function f1(...)
		local args = {...}
		for i=1,#args do
			print(args[i])
			-- assert(args[i] == i*100, args[i])
		end
	end

	local function f2(...)
		local args = {...}
		local args2 = {400,500}
		f1(table.unpack(args),table.unpack(args2)) --100,400,500
	end

	f2(100,200,300)
end

local function test_unpack_args5()
	---创建一个闭包（主要解决函数参数中定义function导致代码太丑）
	---@param func function 包装函数
	---@param ... any 额外参数
	---@return function
	local function callback(func, ...)
		local bind = {...}

		local function bridge(...)
			local args = {...}
			for i=1,#bind do
				table.insert(args, bind[i])
			end

			return func(table.unpack(args))
		end

		return bridge
	end

	local function f1(a,b,c,d,e) 
		assert(a==10)
		assert(b==11)
		assert(c==12)
		assert(d==13)
		assert(e==14)
	end

	local f2 = callback(f1, 12, 13, 14)
	f2(10, 11)
end

local function test_table_remove()
	local tab = {}
	table.insert(tab, 1)
	table.insert(tab, 2)
	table.insert(tab, 3)
	assert(#tab == 3)

	table.remove(tab, 1)
	table.remove(tab, 1)
	table.remove(tab, 1)
	assert(#tab == 0)
end

local function test_debug_getinfo()
	local level = 1
    local w = debug.getinfo(level, "nSlu")
    for k,v in pairs(w) do
        print(k.."="..tostring(v))
    end
end

test_table_remove()
test_table_del_element()
test_table_float_key()
test_table_len()
test_table_insert()
test_string_gmatch()
test_print_loaded_package()
test_table_init2()
test_table_len()
test_var_scope()
test_gmatch()
test_threeop()
test_nil_key()
test_func_param_count()
--test_gfind1()
--test_patterns_1()
test_return_multi_value()
test_global_val()
test_global_val1()
test_sharp()
test_func_call_without_bracket()
test_def_and_use_array()
test_cmp11()
test_os_date()
test_os_time()
test_loadstring()
test_bug01()
test_time_to_0()
test_do_end_block()          
test_for_i()
test_goto()
test_closure()
test_require()
test_load()
test_pairs_order()
test_ENV()
test_single_str_param()
test_class()
test_index()
test_rec_index()
test_str_int_key()
test_coroutine_hello()
test_coroutine_iter1()
test_iter1()
test_iter2()
test___call()
test_cl()
test_int_str_key()
test_weaktable_k()
test_string_find()
test_table_sort2()
test_for_step()
test_number_type()
test_mt_index_001()
test_tab_key_str()
test_weak_key()
test_nil_and_false()
test_ceil()
test_tab_xfsdf3()
test_unpack_args()
test_unpack_args2()
test_unpack_args3()
test_unpack_args4()
test_unpack_args5()
test_debug_getinfo()
test_ipairs_pairs()
