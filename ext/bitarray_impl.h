/* the implementation detail for lua bit array. note all indices start with 0
   in this file */
#pragma once

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>


typedef unsigned int WORD;

/*  number of bits in a word */
#define BITS_PER_WORD (CHAR_BIT * sizeof(WORD))
/* gets the word that contains the bit corresponding to a given index i */
#define I_WORD(i)     ((WORD)(i) / BITS_PER_WORD)
/*  computes a mask to access the correct bit inside this word */
#define I_BIT(i)      (1 << ((WORD)(i) % BITS_PER_WORD))

/* lua userdata for bit array */
typedef struct Bitarray
{
    size_t size;
    WORD *values;
} Bitarray;

/* allocate space to store n bits for ba and set them to 0,
   returns the number of bits available */
static int bitarray_validate(Bitarray *ba, size_t nbits)
{
    ba->values = (WORD *)calloc(I_WORD(nbits - 1) + 1, sizeof(WORD));
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

