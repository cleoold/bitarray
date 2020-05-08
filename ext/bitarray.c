#include "lua.h"
#include "lauxlib.h"

#include "bitarray_impl.h"


#define BITARRAY_INFO "bitarray 1.0 for " LUA_VERSION

#define _BITARRAY_API

#define BITARRAY_MT_1 "cleoold.lua.bitarray_mt1"

/* checks whether the first argument is bitarray */
#define checkbitarray(L) (Bitarray *)luaL_checkudata(L, 1, BITARRAY_MT_1)

_BITARRAY_API static int new(lua_State *L)
{
    size_t nbits = (size_t)luaL_checkinteger(L, 1);
    luaL_argcheck(L, nbits > 0, 1, "invalid size");

    Bitarray *ba = (Bitarray *)lua_newuserdata(L, sizeof(Bitarray));
    if (bitarray_validate(ba, nbits) == 0)
        /* if fails to allocate array */
        return 0;

    luaL_getmetatable(L, BITARRAY_MT_1);
    lua_setmetatable(L, -2);
    return 1;
}

static Bitarray *checkbitarray_and_index(lua_State *L, size_t *i)
{
    Bitarray *ba = checkbitarray(L);
    long i_ = (long)luaL_checkinteger(L, 2) - 1;
    luaL_argcheck(L, 0 <= i_ && i_ < ba->size, 2, "index out of range");
    *i = (size_t)i_;
    return ba;
}

_BITARRAY_API static int setbit(lua_State *L)
{
    size_t i;
    Bitarray *ba = checkbitarray_and_index(L, &i);
    luaL_checkany(L, 3);

    bitarray_set_bit(ba, i, lua_toboolean(L, 3));
    return 0;
}

_BITARRAY_API static int getbit(lua_State *L)
{
    size_t i;
    Bitarray *ba = checkbitarray_and_index(L, &i);

    lua_pushboolean(L, bitarray_get_bit(ba, i));
    return 1;
}

_BITARRAY_API static int len(lua_State *L) {
    Bitarray *ba = checkbitarray(L);
    lua_pushinteger(L, ba->size);
    return 1;
}

_BITARRAY_API static int gc(lua_State *L)
{
    Bitarray *ba = (Bitarray *)luaL_checkudata(L, 1, BITARRAY_MT_1);
    bitarray_invalidate(ba);
    return 0;
}

_BITARRAY_API static int get(lua_State *L)
{
#if LUA_VERSION_NUM >= 503
    if (lua_isinteger(L, 2))
#else
    if (lua_isnumber(L, 2))
#endif
        return getbit(L);
    luaL_getmetatable(L, BITARRAY_MT_1);
    lua_pushvalue(L, 2);
    lua_rawget(L, 3);
    return 1;
}

static const struct luaL_Reg bitarraylib_f[] =
{
    { "new", new },
    { NULL, NULL }
};

static const struct luaL_Reg bitarraylib_m1[] =
{
    { "at", getbit },
    { "set", setbit },
    { "len", len },
    { "__index", get },
    { "__newindex", setbit },
    { "__len", len },
    { "__gc", gc },
    { NULL, NULL }
};

int luaopen_bitarray(lua_State *L)
{
    luaL_newmetatable(L, BITARRAY_MT_1);

#if LUA_VERSION_NUM <= 501 /* for old module system */
    luaL_register(L, NULL, bitarraylib_m1);
    luaL_register(L, "bitarray", bitarraylib_f);
#else /* for above lua 5.2 */
    luaL_setfuncs(L, bitarraylib_m1, 0);
    luaL_newlib(L, bitarraylib_f);
#endif

    lua_pushliteral(L, BITARRAY_INFO);
    lua_setfield(L, -2, "__version");

    return 1;
}
