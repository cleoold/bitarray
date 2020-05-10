package = "bitarray"
version = "1.4-1"
source = {
   url = "git+https://github.com/cleoold/bitarray.git",
   tag = "1.4"
}
description = {
   summary = "Bit array library for lua",
   detailed = [[
       A simple bit array (or bit set, bit string, boolean vector, ... whatever) data structure for Lua written in pure C.
        * Array bit access using overloaded `[]` operators, concatenation with `..`, as well as `&, |, ~` for bit operations (5.3+).
        * Object-oriented access. Method chaining is available.
        * Conversion between bitarray and unsigned integers (big-endian).
    ]],
   homepage = "https://cleoold.github.io/bitarray/",
   license = "MIT"
}
dependencies = {
   "lua >= 5.1"
}
build = {
   type = "builtin",
   modules = {
      bitarray = {
         sources = "ext/bitarray.c"
      }
   },
   copy_directories = { "doc", }
}
