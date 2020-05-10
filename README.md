# [Bitarray](https://cleoold.github.io/bitarray/)

A simple bit array (or bit set, bit string, boolean vector, ... whatever) data structure for Lua written in pure C.

The data structure Bitarray stores bits (booleans) in an array in a memory-saving manner internally. 
One can assign value (false or true) to each element as well as extract values from the array.

* Array bit access using overloaded `[]` operators, concatenation with `..`, as well as `&, |, ~` for bit operations (5.3+).
* Object-oriented access. Method chaining is available.
* Conversion between bitarray and unsigned integers (big-endian).

## Install from [luarocks](https://luarocks.org/modules/cleoold/bitarray)
It will install the release version.
```sh
luarocks install bitarray
```

## Build the library manually
```sh
make all LUA_VERSION=5.3 # or 5.1, 5.2
```
The generated shared library will reside in `out` folder. Unfortunately, I understand the difficulty of finding the right install path
for libraries for different platforms so it is your responsibility to copy the file there. Typically it can be `/usr/local/lib/lua/5.3/`.

## [Documentation](https://cleoold.github.io/bitarray/doc/)
Requiring [ldoc](http://stevedonovan.github.io/ldoc/), available by issuing
```sh
make doc
```
It is also available at [here](doc/index.html).

## Basic usage

Array access
```lua
local Bitarray = require'bitarray'
local a = Bitarray.new(10)
a[1] = true
for i = 1, #a do print(a[i]) end
-- true false false false false false false false false false
```

Method chaining
```lua
local b = Bitarray.new(10)
b:fill(true):set(1, false):set(#b, false)
print(b)
-- Bitarray[0,1,1,1,1,1,1,1,1,0]
```

Storing integers
```lua
local c1 = Bitarray.new(16):from_uint16(65535)
print(c1)
-- Bitarray[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
local c2 = Bitarray.new(16):from_uint16(4015)
print(c2)
-- Bitarray[0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1]
local c3 = c1 .. c2
print(c3)
-- Bitarray[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1]
print(c3:at_uint16(1))
-- 65535
print(c3:at_uint16(17))
-- 4015
print(c3:at_uint32(1))
-- 4294905775
```

## License
MIT
