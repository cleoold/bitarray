#include "lua.h"
#include "lauxlib.h"

#include "bitarray_impl.h"


/**
 * The data structure Bitarray stores bits (booleans) in an array in a
 * memory-saving manner internally. One can assign value (0 or 1) to
 * each element as well as extract values from the array. <br/>
 * Github: <a href="https://github.com/cleoold/bitarray">repo</a>
 * @module bitarray
 */

/**
 * Describes the loaded bitarray library info (version and lua version)
 * @string __version
 */
#define BITARRAY_INFO "bitarray 1.1 for " LUA_VERSION

#define _BITARRAY_API

#define BITARRAY_MT_1 "cleoold.lua.bitarray_mt1"

/* checks whether the first argument is bitarray */
#define checkbitarray(L) (Bitarray *)luaL_checkudata(L, 1, BITARRAY_MT_1)

/**
 * Creates a new bit array of n bits. all fields are initialized to 0.
 * @function new
 * @tparam integer nbits number of bits of the array
 * @treturn Bitarray|nil the newly created bitarray if successful
 */
_BITARRAY_API static int l_new(lua_State *L)
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

/**
 * Creates a new bit array, identical to src.
 * @function copyfrom
 * @tparam Bitarray src
 * @treturn Bitarray|nil the newly created bitarray if successful
 */
_BITARRAY_API static int l_copyfrom(lua_State *L)
{
    Bitarray *ba = checkbitarray(L);

    Bitarray *tg = (Bitarray *)lua_newuserdata(L, sizeof(Bitarray));
    if (bitarray_validate(tg, ba->size) == 0)
        return 0;

    luaL_getmetatable(L, BITARRAY_MT_1);
    lua_setmetatable(L, -2);
    bitarray_copyvalues(ba, (Bitarray *)lua_touserdata(L, -1));
    return 1;
}

/**
 * @type Bitarray
 */

static Bitarray *checkbitarray_and_index(lua_State *L, size_t *i)
{
    Bitarray *ba = checkbitarray(L);
    lua_Integer i_ = luaL_checkinteger(L, 2) - 1;
    luaL_argcheck(L, 0 <= i_ && i_ < ba->size, 2, "index out of range");
    *i = (size_t)i_;
    return ba;
}

/**
 * <i>Mutates the array.</i> <br />
 * Set the ith bit of the array. Any value other than false or nil will be
 * considered a truthy(1) bit. <br />
 * Operator __newindex is overloaded with this method.
 * @function set
 * @tparam integer i the index
 * @tparam any b the value to change to
 * @treturn Bitarray the original bit array reference
 * @usage
 * local a = Bitarray.new(10)
 * a:set(4, true)
 * a[4] = true -- same
 */
_BITARRAY_API static int setbit(lua_State *L)
{
    size_t i;
    Bitarray *ba = checkbitarray_and_index(L, &i);
    luaL_checkany(L, 3);

    bitarray_set_bit(ba, i, lua_toboolean(L, 3));
    lua_pushvalue(L, 1);
    return 1;
}

/**
 * <i>Does not mutate the array.</i> <br />
 * Get the ith bit of the array. <br />
 * Operator __index is overloaded with this method.
 * @function at
 * @tparam integer i the index
 * @treturn boolean true if bit is 1, false if 0
 * @usage
 * local a = Bitarray.new(10)
 * a:at(7) -- false
 * a[7]    -- false
 */
_BITARRAY_API static int getbit(lua_State *L)
{
    size_t i;
    Bitarray *ba = checkbitarray_and_index(L, &i);

    lua_pushboolean(L, bitarray_get_bit(ba, i));
    return 1;
}

/**
 * <i>Does not mutate the array.</i> <br />
 * Get the length of the array. <br />
 * Operator __len is overloaded with this method.
 * @function len
 * @treturn integer the number of bits of the array
 * @usage
 * local a = Bitarray.new(10)
 * a:len(a) -- 10
 * #a       -- 10
 */
_BITARRAY_API static int len(lua_State *L)
{
    Bitarray *ba = checkbitarray(L);
    lua_pushinteger(L, ba->size);
    return 1;
}

/**
 * <i>Mutates the array.</i> <br />
 * Set all bits of the array. Any value other than false or nil will be
 * considered a truthy(1) bit. <br />
 * @function fill
 * @tparam any b the value to change to
 * @treturn Bitarray the original bit array reference
 * @usage
 * local a = Bitarray.new(10)
 * a:fill(true)  -- all bits set to 1
 */
_BITARRAY_API static int fill(lua_State *L)
{
    Bitarray *ba = checkbitarray(L);
    luaL_checkany(L, 2);

    bitarray_fill(ba, lua_toboolean(L, 2));
    lua_pushvalue(L, 1);
    return 1;
}

/**
 * <i>Mutates the array.</i> <br />
 * If argument i is present, the bit at that index is flipped, if not or
 * i is 0, all the bits are flipped.
 * @function flip
 * @tparam[opt] integer i the index
 * @treturn Bitarray the original bit array reference
 * @usage
 * local a = Bitarray.new(10)
 * a:flip()    -- all bits set to 1
 * a:flip(1)   -- sets first bit to true
 */
_BITARRAY_API static int flip(lua_State *L)
{
    Bitarray *ba = checkbitarray(L);
    lua_Integer i = luaL_optinteger(L, 2, 0);
    luaL_argcheck(L, 0 <= i && i <= ba->size, 2, "index out of range");
    if (i == 0)
        bitarray_flip(ba);
    else
        bitarray_flip_bit(ba, (size_t)i - 1);
    lua_pushvalue(L, 1);
    return 1;
}

/**
 * <i>May mutate the array. Does not mutate the existing part. </i> <br />
 * Resize the array to length n. if new size is greater, the new bits
 * are initialized to 0, otherwise additional bits are lost.
 * @function resize
 * @tparam integer n
 * @treturn Bitarray|nil the original bit array reference if successful,
 * otherwise nil
 */
_BITARRAY_API static int resize(lua_State *L)
{
    Bitarray *ba = checkbitarray(L);
    lua_Integer i = luaL_checkinteger(L, 2);
    luaL_argcheck(L, 0 < i, 2, "invalid length");

    if (bitarray_resize(ba, (size_t)i) == 0)
        /* resize failed */
        return 0;
    lua_pushvalue(L, 1);
    return 1;
}

/* finalizer for bitarray */
_BITARRAY_API static int gc(lua_State *L)
{
    Bitarray *ba = (Bitarray *)luaL_checkudata(L, 1, BITARRAY_MT_1);
    bitarray_invalidate(ba);
    return 0;
}

/* actual __index, if param if number it returns result of get(), otherwise
   looks up for fields */
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
    lua_rawget(L, -2);
    return 1;
}

static const struct luaL_Reg bitarraylib_f[] =
{
    { "new", l_new },
    { "copyfrom", l_copyfrom },
    { NULL, NULL }
};

static const struct luaL_Reg bitarraylib_m1[] =
{
    { "at", getbit },
    { "set", setbit },
    { "len", len },
    { "fill", fill },
    { "flip", flip },
    { "resize", resize },
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
