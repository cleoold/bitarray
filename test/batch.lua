package.cpath = 'out/?.so'
local Bitarray = require'bitarray'

local function check(expr)
    if not expr then error('test failed', 2) end
end

local function checkerror(exprf)
    if pcall(exprf) then error('test failed', 2) end
end

-- array creation and set/get bit
do
    local a = Bitarray.new(1)
        check(a:len() == 1)
        check(a.__gc)
    local b = Bitarray.new(2)
        check(b:len() == 2)
    local c = Bitarray.new(7)
        check(#c == 7)
        checkerror(function() return c[8] end)
    local d = Bitarray.new(8)
        d:set(7, true)
        check(d:at(6) == false)
        check(d:at(7) == true)
        check(d:at(8) == false)
    local e = Bitarray.new(9)
        e[8] = true
        e[9] = false
        check(e[8] == true)
        check(e[9] == false)
    local f = Bitarray.new(32)
        f[32] = true
        for i = 1, #f-1 do check(f[i] == false) end
        check(f[32] == true)
        -- chain sets
        f:set(1, true):set(2, false):set(3, true)
        check(f[1] and not f[2] and f[3])
end

-- fill, flip and copy
do
    local a = Bitarray.new(279)
        a:flip()
        for i = 1, 279 do check(a[i]) end
        a:flip(1):flip(279)
        check(not a[1] and a[278] and not a[279])
        a:fill(false)
        for i = 1, 279 do check(not a[i]) end
    local b = Bitarray.copyfrom(a)
        check(#b == #a)
        for i = 1, 279 do check(not b[i]) end
        b:fill(true)
        for i = 1, 279 do check(b[i]) end
        for i = 1, 279 do check(not a[i]) end
end

-- resize
do
    local a = Bitarray.new(1)
        a:resize(4)
        check(not a[1] and not a[2] and not a[3] and not a[4])
        a[4] = true
        a:resize(400)
        check(a[4] and not a[5] and not a[16] and not a[32] and not a[400])
        for i = 210, 400 do a[i] = true end
        a:resize(217)
        check(not a[209] and a[210] and a[217])
        check(#a == 217)
    local b = Bitarray.new(32)
        b:resize(1000)
        for i = 1, 1000 do check(not b[i]) end
        checkerror(function() return b[1001] end)
        b:resize(1024)
        for i = 1, 1024 do check(not b[i]) end
end

-- eq
do
    local a = Bitarray.new(10)
        a:resize(144)
    local b = Bitarray.new(144)
        check(a:equal(b))
        b[5] = true
        check(not a:equal(b))
        a:flip()
        check(a ~= b)
        b:fill(true)
        check(a == b)
        b:resize(143)
        check(a ~= b)
end

-- reverse
do
    local a = Bitarray.new(50)
    local b = Bitarray.copyfrom(a):reverse()
        check(a == b)
        a:set(1, true):set(3, true):set(4, true):set(44, true)
        b:set(50, true):set(48, true):set(47, true):set(7, true):reverse()
        check(a == b)
end

-- slice
do
    local a = Bitarray.new(167):fill(true)
        check(a == a:slice() and a == a:slice(1))
    local b = a:slice(7, 7)
        check(b[1])
    local c = a:slice(1, 128)
        for i = 1, 128 do check(c[i]) end
    local d = a:slice(1, 130)
        for i = 1, 130 do check(d[i]) end
    a[66] = false
    local e = a:slice(26, 68)
        for i = 1, 40 do check(e[i]) end
        check(not e[41])
        for i = 42, 43 do check(e[i]) end
end

print('all tests passed!')
