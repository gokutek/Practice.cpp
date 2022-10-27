local vector3 = {}
local vector4 = {}
local matrix4x4 = {}
local euler = {}
local quat = {}
local transform = {}

---构造vector3
function vector3.new(x, y, z)
	local vec = {}
	vec.x = x or 0
	vec.y = y or 0
	vec.z = z or 0
	vector3.__index = vector3
	setmetatable(vec, vector3)
	return vec
end

---`__tostring`元方法
function vector3:__tostring()
	return "("..self.x..", "..self.y..", "..self.z..")"
end

---返回零向量
---@return vector3
function vector3.zero()
	local vec = vector3.new()
	vec.x = 0
	vec.y = 0
	vec.z = 0
	return vec
end

---求模
---@return number
function vector3:length()
	return math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z)
end

---归一化
---@return vector3
function vector3:normalize()
	local result = vector3.new()
	local len = self:length()
	if len ~= 0 then
		result.x = self.x/len
		result.y = self.y/len
		result.z = self.z/len
	end
	return result
end

---判断两个向量是否相等
---@param vec vector3
---@param epsilon number 允许的误差
---@return boolean
function vector3:equal(vec, epsilon)
	if not epsilon then
		epsilon = 0.00001
	end
	return (math.abs(self.x-vec.x) <= epsilon
			and math.abs(self.y-vec.y) <= epsilon
			and math.abs(self.z-vec.z) <= epsilon)
end

---点乘
---@param vec vector3
---@return number
function vector3:dot(vec)
	return self.x*vec.x + self.y*vec.y + self.z*vec.z
end

---叉乘
---@param vec vector3
---@return vector3
function vector3:cross(vec)
	local result = vector3.new()
	result.x = self.y*vec.z - vec.y*self.z
	result.y = self.z*vec.x - vec.z*self.x
	result.z = self.x*vec.y - vec.x*self.y
	return result
end

---向量+向量
---@param vec vector3
---@return vector3
function vector3:add(vec)
	local result = vector3.new()
	result.x = self.x + vec.x
	result.y = self.y + vec.y
	result.z = self.z + vec.z
	return result
end

---向量-向量
---@param vec vector3
---@return vector3
function vector3:sub(vec)
	local result = vector3.new()
	result.x = self.x - vec.x
	result.y = self.y - vec.y
	result.z = self.z - vec.z
	return result
end

---向量*标量
---@param scalar number
---@return vector3
function vector3:mul(scalar)
	local result = vector3.new()
	result.x = self.x * scalar
	result.y = self.y * scalar
	result.z = self.z * scalar
	return result
end

---求向量在另一个向量上的投影
---@param vec vector3
---@return vector3
function vector3:projection(vec)
	--TODO:
end

-------------------------------------------------------------------------------
---构造vector4
---@return vector4
function vector4.new(x, y, z, w)
	local vec = {}
	vec.x = x or 0
	vec.y = y or 0
	vec.z = z or 0
	vec.w = w or 0
	vector4.__index = vector4
	setmetatable(vec, vector4)
	return vec
end

---`__tostring`元方法
function vector4:__tostring()
	return "("..self.x..", "..self.y..", "..self.z..", "..self.w..")"
end

---返回零向量
---@return vector4
function vector4.zero()
	local vec = vector4.new(0, 0, 0, 0)
	return vec
end

---求模
---@return number
function vector4:length()
	return math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z + self.w*self.w)
end

---归一化
---@return vector4
function vector4:normalize()
	local result = vector4.new()
	local len = self:length()
	if len ~= 0 then
		result.x = self.x/len
		result.y = self.y/len
		result.z = self.z/len
		result.w = self.w/len
	end
	return result
end

---判断两个向量是否相等
---@param vec vector4
---@param epsilon number 允许的误差
---@return boolean
function vector4:equal(vec, epsilon)
	if not epsilon then
		epsilon = 0.00001
	end
	return (math.abs(self.x-vec.x) <= epsilon
			and math.abs(self.y-vec.y) <= epsilon
			and math.abs(self.z-vec.z) <= epsilon
			and math.abs(self.w-vec.w) <= epsilon)
end

---点乘
---@param vec vector4
---@return number
function vector4:dot(vec)
	return self.x*vec.x + self.y*vec.y + self.z*vec.z + self.w*vec.w
end

---向量+向量
---@param vec vector4
---@return vector4
function vector4:add(vec)
	local result = vector4.new()
	result.x = self.x + vec.x
	result.y = self.y + vec.y
	result.z = self.z + vec.z
	result.w = self.w + vec.w
	return result
end

---向量-向量
---@param vec vector4
---@return vector4
function vector4:sub(vec)
	local result = vector4.new()
	result.x = self.x - vec.x
	result.y = self.y - vec.y
	result.z = self.z - vec.z
	result.w = self.w - vec.w
	return result
end

---向量*标量
---@param scalar number
---@return vector4
function vector4:mul(scalar)
	local result = vector4.new()
	result.x = self.x * scalar
	result.y = self.y * scalar
	result.z = self.z * scalar
	result.w = self.w * scalar
	return result
end

---向量x矩阵
---@param matrix matrix4x4
---@return vector4
function vector4:transform(matrix)
	local result = vector4.new()
	result.x = self:dot(matrix:column(1))
	result.y = self:dot(matrix:column(2))
	result.z = self:dot(matrix:column(3))
	result.w = self:dot(matrix:column(4))
	return result
end

-------------------------------------------------------------------------------
---构造一个4x4矩阵
---@return matrix4x4单位矩阵
function matrix4x4.new()
	local m = {}
	m._11 = 1; m._12 = 0; m._13 = 0; m._14 = 0
	m._21 = 0; m._22 = 1; m._23 = 0; m._24 = 0
	m._31 = 0; m._32 = 0; m._33 = 1; m._34 = 0
	m._41 = 0; m._42 = 0; m._43 = 0; m._44 = 1
	matrix4x4.__index = matrix4x4
	setmetatable(m, matrix4x4)
	return m
end

---`__tostring`元方法
function matrix4x4:__tostring()
	local result = ""
	for i=1,4 do
		local row = ""
		for j=1,4 do
			local field = "_"..i..j
			if row ~= "" then
				row = row..", "
			end
			row = row..self[field]
		end
		if result ~= "" then
			result = result.."\n"
		end
		row = "|"..row.."|"
		result = result..row
	end
	return result
end

---构造一个4x4矩阵
---@return matrix4x4单位矩阵
function matrix4x4.identity()
	return matrix4x4.new()
end

---是否相等
---@param matrix matrix4x4
---@param epsilon number 允许的误差
---@return boolean
function matrix4x4:equal(matrix, epsilon)
	if not epsilon then
		epsilon = 0.00001
	end
	for i=1,4 do
		for j=1,4 do
			local field = "_"..i..j
			if math.abs(self[field] - matrix[field]) > epsilon then
				return false
			end
		end
	end
	return true
end

---矩阵求逆
function matrix4x4:inverse()
	--TODO:
end

---矩阵的行列式
function matrix4x4:det()
	--TODO:
end

---矩阵的转置
---@return matrix4x4
function matrix4x4:transpose()
	--TODO:
end

---矩阵x矩阵
function matrix4x4:mul()
	local result = matrix4x4.new()
	for i=1,4 do
		for j=1,4 do
			local field = "_"..i..j
			result[field] = self:row(i):dot(self:column(j))
		end
	end
	return result
end

---平移变换矩阵
---@return matrix4x4
function matrix4x4:translate(x, y, z)
	--TODO:
end

---缩放变换矩阵
---@return matrix4x4
function matrix4x4:scale(x, y, z)
	--TODO:
end

---绕X轴的旋转变换矩阵
---@return matrix4x4
function matrix4x4:rotateX(theta)
	--TODO:
end

---绕Y轴的旋转变换矩阵
---@return matrix4x4
function matrix4x4:rotateY(theta)
	--TODO:
end

---绕Z轴的旋转变换矩阵
---@return matrix4x4
function matrix4x4:rotateZ(theta)
	--TODO:
end

---绕任意轴的旋转变换矩阵
function matrix4x4:rotateAxis(n, theta)
	--TODO:
end

---获取矩阵的某一行
---@param i int 哪一行
---@return vector4
function matrix4x4:row(i)
	assert(i >= 1 and i <= 4)
	if i == 1 then
		return vector4.new(self._11, self._12, self._13. self._14)
	elseif i == 2 then
		return vector4.new(self._21, self._22, self._23. self._24)
	elseif i == 3 then
		return vector4.new(self._31, self._32, self._33. self._34)
	elseif i == 4 then
		return vector4.new(self._41, self._42, self._43. self._44)
	end
end

---获取矩阵的某一列
---@param i int 哪一列
---@return vector4
function matrix4x4:column(i)
	assert(i >= 1 and i <= 4)
	if i == 1 then
		return vector4.new(self._11, self._21, self._31. self._41)
	elseif i == 2 then
		return vector4.new(self._12, self._22, self._32. self._42)
	elseif i == 3 then
		return vector4.new(self._13, self._23, self._33. self._43)
	elseif i == 4 then
		return vector4.new(self._14, self._24, self._34. self._44)
	end
end

-------------------------------------------------------------------------------
---构造欧拉角
---@return euler
function euler.new(pitch, yaw, roll)
	local rotator = {pitch=pitch,yaw=yaw,roll=roll}
	euler.__index = euler
	setmetatable(rotator, euler)
	return rotator
end

-------------------------------------------------------------------------------
---构造单位四元数
---@return 单位四元数
function quat.new()
	local q = {w=1,x=0,y=0,z=0}
	quat.__index = quat
	setmetatable(q, quat)
	return q
end

---根据旋转轴、旋转角度构造四元数
---@param axis vector3 单位向量
---@param angleRad number 弧度
---@return quat
function quat.makeFromAxisAndAngle(axis, angleRad)
	local q = quat.new()
	q.w = math.cos(angleRad/2)
	q.x = math.sin(angleRad/2) * axis.x
	q.y = math.sin(angleRad/2) * axis.y
	q.z = math.sin(angleRad/2) * axis.z
	return q
end

---欧拉角转四元数
---@param rotator euler
---@return quat
function quat.makeFromEuler(rotator)
	--TODO:
end

---矩阵转四元数
---@param matrix matrix4x4
---@return quat
function quat.makeFromMatrix4x4(matrix)
	--TODO:
end

---四元数转欧拉角
---@return euler
function quat:toEuler()
	--TODO:
end

---四元数转矩阵
---@return matrix4x4
function quat:toMatrix4x4()
	--TODO:
end

---四元数插值
---@return quat
function quat.slerp(q1, q2, t)
	--TODO:
end

---四元数对数
---@return quat
function quat:log()
	--TODO:
end

---四元数指数
---@return quat
function quat:exp()
	--TODO:
end

---四元数幂运算
function quat:pow(t)
	--TODO:
end

---四元数x四元数
---@param q quat
---@return quat
function quat:cross(q)
end

---四元数·四元数
---@param q quat
---@return number
function quat:dot(q)
	return self.w*q.w + self.x*q.x + self.y*q.y + self.z*q.z
end

---四元数的模
---@return number
function quat:size()
	return math.sqrt(self.w*self.w + self.x*self.x + self.y*self.y + self.z*self.z)
end

---四元数的共轭
---@return quat
function quat:conjugate()
	local result = quat.new()
	result.w = self.w
	result.x = -self.x
	result.y = -self.y
	result.z = -self.z
	return result
end

---四元数求逆
---@return quat
function quat:inverse()
	local result = self:conjugate()
	local sz = self:size()
	result.w = result.w/sz
	result.x = result.x/sz
	result.y = result.y/sz
	result.z = result.z/sz
	return result
end

---四元数的`差`
---@param q quat 目标四元数
---@return quat
function quat:difference(q)
	--TODO:
end

-------------------------------------------------------------------------------
---构造一个变换
---@return transform
function transform.new()
	local t = {}
	t.scale = vector3.new(1, 1, 1)
	t.translation = vector3.new(0, 0, 0)
	t.rotation = quat.new()
	transform.__index = transform
	setmetatable(t, transform)
end

---转换为矩阵
---@return matrix4x4
function transform:toMatrix()
	--TODO:
end

-------------------------------------------------------------------------------
local function vector3_test()
	local vec1 = vector3.new(1, 2, 3)
	local vec2 = vector3.new(4, 5, 6)
	local result = vec1:dot(vec2)
	assert(result == 1*4+2*5+3*6)
	assert(result == 32)

	--向量与自身的点积，结果也等于自己模的平方
	assert(vec1:dot(vec1) == 14)
	assert(vec1:dot(vec1) == vec1:length()*vec1:length())

	print(vector3.new())
	print(vector4.new())
	print(matrix4x4.new())
end

local function vector4_test()
end

local function matrix4x4_test()
	local m1 = matrix4x4.new()
	local m2 = matrix4x4.identity()
	assert(m1:equal(m2))
end

local function euler_test()
end

local function quat_test()
end

---单元测试
local function math_3d_test()
	vector3_test()
	vector4_test()
	matrix4x4_test()
	euler_test()
	quat_test()
end

math_3d_test()
