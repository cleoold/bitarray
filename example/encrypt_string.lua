-- this is a string encryption example
-- the bitarray simplifies all bitwise tricks into plain array access

local newbitarray = require'bitarray'.new

local function swapitem(t, i, j)
    local tmp = t[i]
    t[i] = t[j]
    t[j] = tmp
end

local function bitarray_fromcharstring(s)
    assert(type(s) == 'string' and s ~= '')
    local bits = newbitarray(#s * 8)
    for i = 1, #s do
        local k = 1 + (i-1) * 8
        bits:from_uint8(s:byte(i), k)
    end
    return bits
end

local function bitarray_tocharstring(bits)
    local res = {}
    for i = 1, #bits, 8 do
        local num = bits:at_uint8(i)
        res[#res+1] = string.char(num)
    end
    return table.concat(res)
end

local function secret(len)
    local mask = newbitarray(len):fill(true)
    for i = 1, len-1, 5 do
        mask:set(i, false):set(i+1, false)
    end
    return mask
end

local function encode(s)
    local bits = bitarray_fromcharstring(s)
    for i = 1, #bits, 8 do
        -- for each 8-bit char, swap 2nd with 3rd, swap 4th with 7th bit
        swapitem(bits, i+1, i+2)
        swapitem(bits, i+3, i+6)
    end
    bits:reverse()
    bits = bits:bxor(secret(#bits))
    -- rebuild the string
    return bitarray_tocharstring(bits)
end

local function decode(s)
    local bits = bitarray_fromcharstring(s)
    bits = bits:bxor(secret(#bits))
    bits:reverse()
    for i = 1, #bits, 8 do
        swapitem(bits, i+1, i+2)
        swapitem(bits, i+3, i+6)
    end
    return bitarray_tocharstring(bits)
end

return { encode = encode, decode = decode }
