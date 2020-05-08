package.cpath = 'out/?.so'
local Bitarray = require'bitarray'

local function check(expr)
    if not expr then error('test failed', 2) end
end

-- array creation and set/get bit

local a = Bitarray.new(1)
    check(a:len() == 1)
    check(a.__gc)
local b = Bitarray.new(2)
    check(b:len() == 2)
local c = Bitarray.new(7)
    check(#c == 7)
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

print('all tests passed!')
