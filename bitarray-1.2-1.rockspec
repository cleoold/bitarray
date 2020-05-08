package = "bitarray"
version = "1.2-1"
source = {
   url = "git+https://github.com/cleoold/bitarray.git",
   tag = "1.2"
}
description = {
   summary = "Bit array library for lua",
   detailed = "A simple bit array (or bit set, bit string, boolean vector, ... whatever) data structure for Lua written in pure C.",
   homepage = "https://github.com/cleoold/bitarray",
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
   }
}
