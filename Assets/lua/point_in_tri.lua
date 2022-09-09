-- print("Waiting for debugger to attach...")
-- a = io.read("l")

-------------------------------------------------------------------------------

local Vector2D = {}

function Vector2D.new(x, y)
    local v = {X=x, Y=y}
    Vector2D.__index = Vector2D
    setmetatable(v, Vector2D)
    return v
end

function Vector2D:dot(v)
    return self.X*v.X + self.Y*v.Y
end

function Vector2D:__sub(v)
    return Vector2D.new(self.X-v.X, self.Y-v.Y)
end

function Vector2D:__tostring()
    return "{X="..self.X..",Y="..self.Y.."}"
end

-------------------------------------------------------------------------------

local Vector3D = {}

function Vector3D.new(x, y, z)
    local v = {X=x, Y=y, Z=z}
    Vector3D.__index = Vector3D
    setmetatable(v, Vector3D)
    return v
end

function Vector3D:dot(v)
    return self.X*v.X + self.Y*v.Y + self.Z*v.Z
end

function Vector3D:cross(v)
    return Vector3D.new(self.Y*v.Z-self.Z*v.Y, 
                        self.Z*v.X-self.X*v.Z, 
                        self.X*v.Y-self.Y*v.X)
end

function Vector3D:__sub(v)
    return Vector3D.new(self.X-v.X, self.Y-v.Y, self.Z-v.Z)
end

function Vector3D:__tostring()
    return "{X="..self.X..",Y="..self.Y..",Z="..self.Z.."}"
end

local function Vector3DTest()
    local axisX = Vector3D.new(1, 0, 0)
    local axisY = Vector3D.new(0, 1, 0)
    local axisZ = axisX:cross(axisY)
    assert(axisZ.Z == 1)

    axisZ = axisY:cross(axisX)
    assert(axisZ.Z == -1)
end

Vector3DTest()

-------------------------------------------------------------------------------

function PointInTriangle(A, B, C, P)
    -- Compute vectors        
    local v0 = C - A
    local v1 = B - A
    local v2 = P - A

    -- Compute dot products
    local dot00 = v0:dot(v0)
    local dot01 = v0:dot(v1)
    local dot02 = v0:dot(v2)
    local dot11 = v1:dot(v1)
    local dot12 = v1:dot(v2)

    -- Compute barycentric coordinates
    local invDenom = 1 / (dot00 * dot11 - dot01 * dot01)
    local u = (dot11 * dot02 - dot01 * dot12) * invDenom
    local v = (dot00 * dot12 - dot01 * dot02) * invDenom

    -- Check if point is in triangle
    return (u >= 0) and (v >= 0) and (u + v < 1)
end

function PointInTriangle2(A, B, C, P)
    -- Compute vectors        
    local v0 = C - A
    local v1 = B - A
    local v2 = P - A

    local a=v0.X
    local b=v1.X
    local e=v2.X
    
    local c=v0.Y
    local d=v1.Y
    local f=v2.Y

    local u = (d*e-b*f)/(a*d-b*c)
    local v = (a*f-c*e)/(a*d-b*c)

    return Vector2D.new(u, v)
end

local A=Vector2D.new(0, 0)
local B=Vector2D.new(100, 0)
local C=Vector2D.new(50, 100)
assert(true == PointInTriangle(A, B, C, Vector2D.new(50, 10)))
assert(true == PointInTriangle(A, B, C, Vector2D.new(50, 90)))
assert(false == PointInTriangle(A, B, C, Vector2D.new(50, 100)))
assert(false == PointInTriangle(A, B, C, Vector2D.new(110, 45211)))

print(PointInTriangle2(A, B, C, Vector2D.new(50, 10)))
print(PointInTriangle2(B, C, A, Vector2D.new(50, 10)))
print(PointInTriangle2(C, B, A, Vector2D.new(50, 10)))

do
    function SameSide(p1, p2, a, b)
        local x = b-a
        local cp1 = x:cross(p1-a)
        local cp2 = x:cross(p2-a)
        local d = cp1:dot(cp2)
        if d >= 0 then 
            return true
        else 
            return false
        end
    end
    
    function PointInTriangle0(p, a,b,c)
        if SameSide(p,a, b,c) and SameSide(p,b, a,c) and SameSide(p,c, a,b) then
             return true
        else 
            return false
        end
    end

    local A = Vector3D.new(0, 0, 0)
    local B = Vector3D.new(100, 0, 0)
    local C = Vector3D.new(50, 100, 0)
    assert(true == PointInTriangle0(Vector3D.new(50, 10, 0), A, B, C))
    assert(false == PointInTriangle0(Vector3D.new(200, 10, 0), A, B, C))
end
