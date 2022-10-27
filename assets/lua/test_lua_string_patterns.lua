--[[
    http://www.lua.org/manual/5.4/manual.html#6.4.1

character class:
pattern item:
pattern:
--]]

local function test_pattern0()
    local fileName = "newfile.txt"
    --x: (where x is not one of the magic characters ^$()%.[]*+-?) represents the character x itself.
    local start = string.find(fileName, "f")
    assert(start == 4)
end

local function test_pattern1()
    local fileName = "newfile.txt"
    --%转义
    local start = string.find(fileName, "%.")
    assert(start == 8)
end

--%a: represents all letters.
local function test_pattern2()
    local str = "1234567890ABC"
    local start = string.find(str, "%a")
    assert(start == 11)
end

--%c: represents all control characters.
local function test_pattern3()
    local str = "hello\nworld"
    local start = string.find(str, "%c")
    assert(start == 6)
end

--%d: represents all digits.
local function test_pattern4()
    local str = "hello1world"
    local start = string.find(str, "%d")
    assert(start == 6)
end

--%g: represents all printable characters except space.
local function test_pattern5()
    local str = "\n\n\n\n\nH"
    local start = string.find(str, "%g")
    assert(start == 6)
end

--%l: represents all lowercase letters.
local function test_pattern6()
    local str = "HELLOworld"
    local start = string.find(str, "%l")
    assert(start == 6)
end

--%p: represents all punctuation characters.
local function test_pattern7()
    local str = "HELLO,world"
    local start = string.find(str, "%p")
    assert(start == 6)
end

--%s: represents all space characters.
local function test_pattern8()
    local str = "HELLO world"
    local start = string.find(str, "%s")
    assert(start == 6)
end

--%u: represents all uppercase letters.
local function test_pattern9()
    local str = "helloWorld"
    local start = string.find(str, "%u")
    assert(start == 6)
end

--%w: represents all alphanumeric characters.
local function test_pattern10()
    local str = "-,.*)World"
    local start = string.find(str, "%w")
    assert(start == 6)

    local str = "-,.*)12345"
    local start = string.find(str, "%w")
    assert(start == 6)
end

--%x: represents all hexadecimal digits.
local function test_pattern11()
    local str = "TTTTTA"
    local start = string.find(str, "%x")
    assert(start == 6)

    local str = "TTTTTB"
    local start = string.find(str, "%x")
    assert(start == 6)

    local str = "TTTTTC"
    local start = string.find(str, "%x")
    assert(start == 6)

    local str = "TTTTTD"
    local start = string.find(str, "%x")
    assert(start == 6)

    local str = "TTTTTE"
    local start = string.find(str, "%x")
    assert(start == 6)

    local str = "TTTTTF"
    local start = string.find(str, "%x")
    assert(start == 6)

    local str = "TTTTTa"
    local start = string.find(str, "%x")
    assert(start == 6)

    local str = "TTTTTX"
    local start = string.find(str, "%x")
    assert(start == nil)
end

--%x: (where x is any non-alphanumeric character) represents the character x.
local function test_pattern12()
    local str = "hello*"
    local start = string.find(str, "%*")
    assert(start == 6)

    local str = "hello*"
    local start = string.find(str, "*")
    assert(start == 6)

    --主要用途时用来转义（^$()%.[]*+-?）这些字符
end

--[set]
local function test_pattern13()
    local str = "helloworld"
    local start = string.find(str, "[abcde]")
    assert(start == 2)
end

--[^set]
local function test_pattern14()
    local str = "abcde123"
    local start = string.find(str, "[^abcde]")
    assert(start == 6)
end

-------------------------------------------------------------------------------

--a single character class, which matches any single character in the class;
local function test_pattern_item0()
end

--a single character class followed by '*', which matches sequences of zero or more characters in the class. 
--These repetition items will always match the longest possible sequence;
local function test_pattern_item1()
    local str = "abc123defg"
    local start, ends = string.find(str, "%l*")
    assert(start == 1)
    assert(ends == 3)
end

--a single character class followed by '+', which matches sequences of one or more characters in the class. 
--These repetition items will always match the longest possible sequence;
local function test_pattern_item2()
end

--a single character class followed by '-', which also matches sequences of zero or more characters in the class. 
--Unlike '*', these repetition items will always match the shortest possible sequence;
local function test_pattern_item3()
end

--a single character class followed by '?', which matches zero or one occurrence of a character in the class. 
--It always matches one occurrence if possible;
local function test_pattern_item4()
end

--%n, for n between 1 and 9; such item matches a substring equal to the n-th captured string (see below);
local function test_pattern_item5()
end

--%bxy, where x and y are two distinct characters; such item matches strings that start with x, end with y, and where the x and y are balanced. 
--This means that, if one reads the string from left to right, counting +1 for an x and -1 for a y, the ending y is the first y where the count reaches 0. 
--For instance, the item %b() matches expressions with balanced parentheses.
local function test_pattern_item6()
end

--%f[set], a frontier pattern; such item matches an empty string at any position such that the next character belongs to set and the previous character does not belong to set. 
--The set set is interpreted as previously described. The beginning and the end of the subject are handled as if they were the character '\0
local function test_pattern_item7()
end

test_pattern0()
test_pattern1()
test_pattern2()
test_pattern3()
test_pattern4()
test_pattern5()
test_pattern6()
test_pattern7()
test_pattern8()
test_pattern9()
test_pattern10()
test_pattern11()
test_pattern12()
test_pattern13()
test_pattern14()

test_pattern_item0()
test_pattern_item1()
test_pattern_item2()
test_pattern_item3()
test_pattern_item4()
test_pattern_item5()
test_pattern_item6()
test_pattern_item7()
