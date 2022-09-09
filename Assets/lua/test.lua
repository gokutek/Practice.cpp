function OnClicked(Image)
end

local function vector_add(vec1, vec2)
    local result = create_vector(vec1.x + vec2.x, vec1.y + vec2.y)
    return result
end

local function vector_index(vec, key)
    print("vector_index")
    if key == 1 then
        return vec.x
    elseif key == 2 then
        return vec.y
    end
end

local function vector_new_index(vec, key, value)
    print("vector_new_index")
    -- vec[key] = value
    rawset(vec, key, value)
end

function create_vector(x, y)
    local vec = {}
    vec.x = x
    vec.y = y

    local metatable = {}
    metatable["__add"] = vector_add
    metatable["__index"] = vector_index
    metatable["__newindex"] = vector_new_index

    setmetatable(vec, metatable)
    return vec
end

function script_main()
    local vec1 = create_vector(1, 2)
    local vec2 = create_vector(3, 4)
    local vec3 = vec1 + vec2
    print("x=" .. vec3.x .. ", y=" .. vec3.y)
    print("x=" .. vec3[1] .. ", y=" .. vec3[2])

    print(vec3[10])
    print(vec3[10])
    vec3[10] = 1
    print(vec3[10])
    vec3[10] = 1

end

script_main()
