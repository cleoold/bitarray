/* the implementation detail for lua bit array. note all indices start with 0
   in this file */
#pragma once

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>


typedef unsigned int WORD;

/*  number of bits in a word */
#define BITS_PER_WORD     (CHAR_BIT * sizeof(WORD))
/* gets the word that contains the bit corresponding to a given index i */
#define I_WORD(i)         ((WORD)(i) / BITS_PER_WORD)
/*  computes a mask to access the correct bit inside this word */
#define I_BIT(i)          (1 << ((WORD)(i) % BITS_PER_WORD))
/* computes how many words to store n bits */
#define WORDS_FOR_BITS(n) (I_WORD((n) - 1) + 1)

/* lua userdata for bit array
   must note all unused bit positions have to be 0 at all times*/
typedef struct Bitarray
{
    size_t size;
    WORD *values;
} Bitarray;

/* allocate space to store n bits for ba and set them to 0,
   returns the number of bits available */
static int bitarray_validate(Bitarray *ba, size_t nbits)
{
    ba->values = (WORD *)calloc(WORDS_FOR_BITS(nbits), sizeof(WORD));
    if (ba->values != NULL)
        return ba->size = nbits;
    return 0;
}

static void bitarray_invalidate(Bitarray *ba)
{
    free(ba->values);
    ba->size = 0;
}

/* given an index, returns the word address and the mask to access the bit */
static WORD *bitarray_get_bit_access(Bitarray *ba, size_t i, WORD *mask)
{
    if (mask != NULL)
        *mask = I_BIT(i);
    return &ba->values[I_WORD(i)];
}

/* set ith bit to 1 if b is truthy, else 0 */
static void bitarray_set_bit(Bitarray *ba, size_t i, int b)
{
    WORD mask;
    WORD *word = bitarray_get_bit_access(ba, i, &mask);
    if (b)
        *word |= mask; /* set bit */
    else
        *word &= ~mask; /* reset bit */
}

/* get ith bit (1 or 0) */
static int bitarray_get_bit(Bitarray *ba, size_t i)
{
    WORD mask;
    WORD *word = bitarray_get_bit_access(ba, i, &mask);
    return (*word & mask) ? 1 : 0;
}

/* 1 -> 0 and 0 -> 1 */
static void bitarray_flip_bit(Bitarray *ba, size_t i)
{
    WORD mask;
    WORD *word = bitarray_get_bit_access(ba, i, &mask);
    *word = (*word & mask) ? (*word & ~mask) : (*word | mask);
}

/* for loop that always set the unused bits to 0 */
#define BITARRAY_WORD_ITER(ba, EXPR) do { \
    size_t nwords = WORDS_FOR_BITS((ba)->size); \
    for (size_t i = 0; i < nwords; ++i) { \
        EXPR } \
    for (size_t i = (ba)->size; i < nwords * BITS_PER_WORD; ++i) \
        bitarray_set_bit((ba), i, 0); } while(0) \

static void bitarray_flip(Bitarray *ba)
{
    BITARRAY_WORD_ITER(ba,
        ba->values[i] = ~ba->values[i];
    );
}

/* set all bits to 1 if b is truthy, else 0 */
static void bitarray_fill(Bitarray *ba, int b)
{
    WORD bb = b ? (WORD)-1 : 0;
    BITARRAY_WORD_ITER(ba,
        ba->values[i] = bb;
    );
}

/* resize the array. if new size is bigger, fill the new bit positions with 0.
   also set any unused bits to 0 (ie the gap between size and the actual end
   of WORDs). returns the new size, or 0 is returned if failed (array unchanged)*/
static int bitarray_resize(Bitarray *ba, size_t nbits)
{
    if (nbits == ba->size)
        return nbits;
    size_t oldwords = WORDS_FOR_BITS(ba->size);
    size_t newwords = WORDS_FOR_BITS(nbits);
    if (oldwords != newwords) {
        WORD *tmp = (WORD *)realloc(ba->values, newwords * sizeof(WORD));
        if (tmp == NULL)
            return 0;
        ba->values = tmp;
    }
    size_t oldbits = ba->size;
    ba->size = nbits;
    if (nbits < oldbits) {
        for (size_t i = nbits; i < newwords * BITS_PER_WORD; ++i)
            bitarray_set_bit(ba, i, 0);
    } else {
        for (size_t i = oldbits; i < oldwords * BITS_PER_WORD; ++i)
            bitarray_set_bit(ba, i, 0);
        for (size_t i = oldwords; i < newwords; ++i)
            ba->values[i] = 0;
    }
    return nbits;
}

/* copy values from ba to tg */
static void bitarray_copyvalues(Bitarray *ba, Bitarray *tg)
{
    for (size_t i = 0; i < WORDS_FOR_BITS(ba->size); ++i)
        tg->values[i] = ba->values[i];
}

