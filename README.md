# [Bitarray](https://cleoold.github.io/bitarray/)

A simple bit array (or bit set, bit string, boolean vector, ... whatever) data structure for Lua written in pure C.

The data structure Bitarray stores bits (booleans) in an array in a memory-saving manner internally. 
One can assign value (0 or 1) to each element as well as extract values from the array.

## Install from luarocks
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

## Basic usage
```lua
local Bitarray = require'bitarray'
local a = Bitarray.new(10)
a[1] = true
for i = 1, #a do print(a[i]) end
-- true false false false false false false false false false
```

## [Documentation](doc/index.html)
Requiring [ldoc](http://stevedonovan.github.io/ldoc/), available by issuing
```sh
make doc
```
It is also available at [here](doc/index.html).
