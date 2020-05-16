#pragma once

#define BITARRAY_API

#if (defined(_WIN32) || defined(__CYGWIN__) || defined(__MINGW32__))
    #define LUA_BUILD_AS_DLL
    #define BITARRAY_MAIN __declspec(dllexport)
#else
    #define BITARRAY_MAIN
#endif

#include "lua.h"
#include "lauxlib.h"
