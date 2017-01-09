/*!
 * \file summary/bitmap.h
 *
 * \date Sep 24, 2013
 * \author Ricardo C. Corrêa (correa@lia.ufc.br)
 *
 * \brief Macros and inline functions for bitwise operations on bitmaps using bit-parallelism.
 *
 * A bitmap is an array of bits to represent sets in which a bit of index \c i represents element \c i of the set. For a bitmap \c B,
 * \c B[\c i] \c = \c 1 means that element \c i is in the set; otherwise, \c i is not.
 *
 * Bitmaps are defined as arrays of three different types depending on the number of bits per node, namely: \c unsigned \c char (8 bits
 * per node), \c unsigned \c long \c int (32 bits per node), and \c unsigned \c long \c long \c int (64 bits per node). The type
 * currently in use is selected by means of the definition of a macro name, respectively with \c -D_ARCH8_, \c -D_ARCH32_, or
 * \c -D_ARCH64_, in the \c gcc command line.
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////////////
#ifdef _ARCH256_
#include <immintrin.h>//AVX

typedef union {
    __m256d md;
    __m256i mi;
    unsigned long long int ui64[4];
} NODETYPE; //!< bitmap node's type

static const NODETYPE __zero = { .ui64 = { 0, 0, 0, 0 } };

#define BSIZE 256
#define WSIZE 32
#define LOGB 8LL
#define LOGW 5
#define LOGBW 13

#define ONE 1LL
#define NODEMAX 0xFFFFFFFFFFFFFFFF
#define MASKB 0xFFLL
//#define MASKB 0xFFFFFFFFFFFFFFC0

#define MASKSZ (BSIZE+1)
static const unsigned long long __mask[MASKSZ] = {
	0xFFFFFFFFFFFFFFFF,
	0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFF8, 0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFFE0, 0xFFFFFFFFFFFFFFC0, 0xFFFFFFFFFFFFFF80, 0xFFFFFFFFFFFFFF00,
	0xFFFFFFFFFFFFFE00, 0xFFFFFFFFFFFFFC00, 0xFFFFFFFFFFFFF800, 0xFFFFFFFFFFFFF000,
	0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFC000, 0xFFFFFFFFFFFF8000, 0xFFFFFFFFFFFF0000,
	0xFFFFFFFFFFFE0000, 0xFFFFFFFFFFFC0000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFF00000,
	0xFFFFFFFFFFE00000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFF800000, 0xFFFFFFFFFF000000,
	0xFFFFFFFFFE000000, 0xFFFFFFFFFC000000, 0xFFFFFFFFF8000000, 0xFFFFFFFFF0000000,
	0xFFFFFFFFE0000000, 0xFFFFFFFFC0000000, 0xFFFFFFFF80000000, 0xFFFFFFFF00000000,
	0xFFFFFFFE00000000, 0xFFFFFFFC00000000, 0xFFFFFFF800000000, 0xFFFFFFF000000000,
	0xFFFFFFE000000000, 0xFFFFFFC000000000, 0xFFFFFF8000000000, 0xFFFFFF0000000000,
	0xFFFFFE0000000000, 0xFFFFFC0000000000, 0xFFFFF80000000000, 0xFFFFF00000000000,
	0xFFFFE00000000000, 0xFFFFC00000000000, 0xFFFF800000000000, 0xFFFF000000000000,
	0xFFFE000000000000, 0xFFFC000000000000, 0xFFF8000000000000, 0xFFF0000000000000,
	0xFFE0000000000000, 0xFFC0000000000000, 0xFF80000000000000, 0xFF00000000000000,
	0xFE00000000000000, 0xFC00000000000000, 0xF800000000000000, 0xF000000000000000,
	0xE000000000000000, 0xC000000000000000, 0x8000000000000000, 0xFFFFFFFFFFFFFFFF,
	0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFF8, 0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFFE0, 0xFFFFFFFFFFFFFFC0, 0xFFFFFFFFFFFFFF80, 0xFFFFFFFFFFFFFF00,
	0xFFFFFFFFFFFFFE00, 0xFFFFFFFFFFFFFC00, 0xFFFFFFFFFFFFF800, 0xFFFFFFFFFFFFF000,
	0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFC000, 0xFFFFFFFFFFFF8000, 0xFFFFFFFFFFFF0000,
	0xFFFFFFFFFFFE0000, 0xFFFFFFFFFFFC0000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFF00000,
	0xFFFFFFFFFFE00000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFF800000, 0xFFFFFFFFFF000000,
	0xFFFFFFFFFE000000, 0xFFFFFFFFFC000000, 0xFFFFFFFFF8000000, 0xFFFFFFFFF0000000,
	0xFFFFFFFFE0000000, 0xFFFFFFFFC0000000, 0xFFFFFFFF80000000, 0xFFFFFFFF00000000,
	0xFFFFFFFE00000000, 0xFFFFFFFC00000000, 0xFFFFFFF800000000, 0xFFFFFFF000000000,
	0xFFFFFFE000000000, 0xFFFFFFC000000000, 0xFFFFFF8000000000, 0xFFFFFF0000000000,
	0xFFFFFE0000000000, 0xFFFFFC0000000000, 0xFFFFF80000000000, 0xFFFFF00000000000,
	0xFFFFE00000000000, 0xFFFFC00000000000, 0xFFFF800000000000, 0xFFFF000000000000,
	0xFFFE000000000000, 0xFFFC000000000000, 0xFFF8000000000000, 0xFFF0000000000000,
	0xFFE0000000000000, 0xFFC0000000000000, 0xFF80000000000000, 0xFF00000000000000,
	0xFE00000000000000, 0xFC00000000000000, 0xF800000000000000, 0xF000000000000000,
	0xE000000000000000, 0xC000000000000000, 0x8000000000000000, 0xFFFFFFFFFFFFFFFF,
	0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFF8, 0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFFE0, 0xFFFFFFFFFFFFFFC0, 0xFFFFFFFFFFFFFF80, 0xFFFFFFFFFFFFFF00,
	0xFFFFFFFFFFFFFE00, 0xFFFFFFFFFFFFFC00, 0xFFFFFFFFFFFFF800, 0xFFFFFFFFFFFFF000,
	0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFC000, 0xFFFFFFFFFFFF8000, 0xFFFFFFFFFFFF0000,
	0xFFFFFFFFFFFE0000, 0xFFFFFFFFFFFC0000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFF00000,
	0xFFFFFFFFFFE00000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFF800000, 0xFFFFFFFFFF000000,
	0xFFFFFFFFFE000000, 0xFFFFFFFFFC000000, 0xFFFFFFFFF8000000, 0xFFFFFFFFF0000000,
	0xFFFFFFFFE0000000, 0xFFFFFFFFC0000000, 0xFFFFFFFF80000000, 0xFFFFFFFF00000000,
	0xFFFFFFFE00000000, 0xFFFFFFFC00000000, 0xFFFFFFF800000000, 0xFFFFFFF000000000,
	0xFFFFFFE000000000, 0xFFFFFFC000000000, 0xFFFFFF8000000000, 0xFFFFFF0000000000,
	0xFFFFFE0000000000, 0xFFFFFC0000000000, 0xFFFFF80000000000, 0xFFFFF00000000000,
	0xFFFFE00000000000, 0xFFFFC00000000000, 0xFFFF800000000000, 0xFFFF000000000000,
	0xFFFE000000000000, 0xFFFC000000000000, 0xFFF8000000000000, 0xFFF0000000000000,
	0xFFE0000000000000, 0xFFC0000000000000, 0xFF80000000000000, 0xFF00000000000000,
	0xFE00000000000000, 0xFC00000000000000, 0xF800000000000000, 0xF000000000000000,
	0xE000000000000000, 0xC000000000000000, 0x8000000000000000, 0xFFFFFFFFFFFFFFFF,
	0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFF8, 0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFFE0, 0xFFFFFFFFFFFFFFC0, 0xFFFFFFFFFFFFFF80, 0xFFFFFFFFFFFFFF00,
	0xFFFFFFFFFFFFFE00, 0xFFFFFFFFFFFFFC00, 0xFFFFFFFFFFFFF800, 0xFFFFFFFFFFFFF000,
	0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFC000, 0xFFFFFFFFFFFF8000, 0xFFFFFFFFFFFF0000,
	0xFFFFFFFFFFFE0000, 0xFFFFFFFFFFFC0000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFF00000,
	0xFFFFFFFFFFE00000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFF800000, 0xFFFFFFFFFF000000,
	0xFFFFFFFFFE000000, 0xFFFFFFFFFC000000, 0xFFFFFFFFF8000000, 0xFFFFFFFFF0000000,
	0xFFFFFFFFE0000000, 0xFFFFFFFFC0000000, 0xFFFFFFFF80000000, 0xFFFFFFFF00000000,
	0xFFFFFFFE00000000, 0xFFFFFFFC00000000, 0xFFFFFFF800000000, 0xFFFFFFF000000000,
	0xFFFFFFE000000000, 0xFFFFFFC000000000, 0xFFFFFF8000000000, 0xFFFFFF0000000000,
	0xFFFFFE0000000000, 0xFFFFFC0000000000, 0xFFFFF80000000000, 0xFFFFF00000000000,
	0xFFFFE00000000000, 0xFFFFC00000000000, 0xFFFF800000000000, 0xFFFF000000000000,
	0xFFFE000000000000, 0xFFFC000000000000, 0xFFF8000000000000, 0xFFF0000000000000,
	0xFFE0000000000000, 0xFFC0000000000000, 0xFF80000000000000, 0xFF00000000000000,
	0xFE00000000000000, 0xFC00000000000000, 0xF800000000000000, 0xF000000000000000,
	0xE000000000000000, 0xC000000000000000, 0x8000000000000000, 0x0000000000000000
};

static const unsigned long long __oneshifted[MASKSZ] = {
	0x0000000000000001,
	0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010,
	0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100,
	0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000,
	0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000,
	0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000,
	0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000,
	0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000,
	0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000,
	0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000,
	0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000,
	0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000,
	0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000,
	0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000,
	0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000,
	0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000,
	0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000001,
	0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010,
	0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100,
	0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000,
	0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000,
	0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000,
	0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000,
	0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000,
	0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000,
	0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000,
	0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000,
	0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000,
	0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000,
	0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000,
	0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000,
	0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000,
	0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000001,
	0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010,
	0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100,
	0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000,
	0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000,
	0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000,
	0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000,
	0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000,
	0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000,
	0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000,
	0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000,
	0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000,
	0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000,
	0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000,
	0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000,
	0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000,
	0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000001,
	0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010,
	0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100,
	0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000,
	0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000,
	0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000,
	0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000,
	0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000,
	0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000,
	0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000,
	0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000,
	0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000,
	0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000,
	0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000,
	0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000,
	0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000,
	0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000000
};

static const char __index[MASKSZ] = {
	0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2, 2, 2, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 3,
	3, 3, 3, 4
};

/*
 * Returns the number of 1-bits in x.
 */
static inline int __builtin_popcountAVX(NODETYPE x) {
	return __builtin_popcountll(x.ui64[0]) + __builtin_popcountll(x.ui64[1]) + __builtin_popcountll(x.ui64[2]) + __builtin_popcountll(x.ui64[3]);
}

/*
 * Returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero.
 */
static inline int __builtin_ffsAVX(NODETYPE x){
	if (x.ui64[0] != 0)
		return __builtin_ffsll(x.ui64[0]);
	if (x.ui64[1] != 0)
		return __builtin_ffsll(x.ui64[1]) + 64;
	if (x.ui64[2] != 0)
		return __builtin_ffsll(x.ui64[2]) + 128;
	if (x.ui64[3] != 0)
		return __builtin_ffsll(x.ui64[3]) + 192;
	return 0;
}


/*
 * Returns one plus the index of the significant 1-bit of x next to i, or if x is zero, returns zero.
 */
static inline int __builtin_nextffsAVX(NODETYPE x, int i) {
	unsigned long long int masked;

	i++;
	if (i < 64)
	{
		masked = x.ui64[0] & __mask[i];
		if (masked != 0)
			return __builtin_ffsll(masked);
		i = 64;
	}
	if (i < 128)
	{
		masked = x.ui64[1] & __mask[i];
		if (masked != 0)
			return __builtin_ffsll(masked) + 64;
		i = 128;
	}
	if (i < 198)
	{
		masked = x.ui64[2] & __mask[i];
		if(masked !=0)
			return __builtin_ffsll(masked) + 128;
		i = 192;
	}
	masked = x.ui64[3] & __mask[i];
	if (masked == 0)
		return 0;
	return __builtin_ffsll(masked) + 192;
}

static inline int __builtin_prevclzAVX(NODETYPE x, int i) {
	unsigned long long int masked;

	if (i > 192)
	{
		masked = x.ui64[3] & ~__mask[i];
		if (masked != 0)
			return __builtin_clzll(masked);
		i = 128;
	}
	if (i > 128)
	{
		masked = x.ui64[2] & ~__mask[i];
		if (masked != 0)
			return __builtin_clzll(masked) + 64;
		i = 128;
	}
	if (i > 64)
	{
		masked = x.ui64[1] & ~__mask[i];
		if (masked != 0)
			return __builtin_clzll(masked) + 128;
		i = 64;
	}
	masked = x.ui64[0] & ~__mask[i];
	if (masked == 0)
		return BSIZE;
	return __builtin_clzll(masked) + 192;
}

static inline int __builtin_clzAVX(NODETYPE x) {
	if (x.ui64[3] != 0)
		return __builtin_clzll(x.ui64[3]);
	if (x.ui64[2] != 0)
		return __builtin_clzll(x.ui64[2]) + 64;
	if (x.ui64[1] != 0)
		return __builtin_clzll(x.ui64[1]) + 128;
	return __builtin_clzll(x.ui64[0]) + 192;
}


#define NODECOUNTER(i) __builtin_popcountAVX(i)
#define NODELEAST(w) (__builtin_ffsAVX(w)-1)
#define NODENEXTLEAST(w, i) (__builtin_nextffsAVX((w), (i))-1)
#define NODEMOST(i) (BSIZE-__builtin_clzAVX(i)-1)
#define NODEPREVMOST(w, i) (BSIZE-__builtin_prevclzAVX((w), (i))-1)
#define NODEISEMPTY(w) ((w).ui64[0] == 0 && (w).ui64[1] == 0 && (w).ui64[2] == 0  && (w).ui64[3] == 0 )
#define NODEISNOTEMPTY(w) ((w).ui64[0] != 0 || (w).ui64[1] != 0 || (w).ui64[2] != 0 || (w).ui64[3] != 0)
#define ISINNODE(w, i) ((w) & __oneshifted[i])							//!< Returns a node set to a non-zero value if the bit of index \c i in node \c w is a 1-bit; otherwise a node set to \c 0.

#elif _ARCH128_
#include <smmintrin.h> // SSE4

typedef union {
    __m128i mi;
    __m128 m;
    unsigned long long int ui64[2];
} NODETYPE; //!< bitmap node's type

static const NODETYPE __zero = { .ui64 = { 0, 0 } };

#define BSIZE 128
#define WSIZE 16
#define LOGB 7LL
#define LOGW 4
#define LOGBW 11

#define ONE 1LL
#define NODEMAX 0xFFFFFFFFFFFFFFFF
#define MASKB 0x7FLL
//#define MASKB 0xFFFFFFFFFFFFFFC0

#define MASKSZ (BSIZE+1)
static const unsigned long long __mask[MASKSZ] = {
	0xFFFFFFFFFFFFFFFF,
	0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFF8, 0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFFE0, 0xFFFFFFFFFFFFFFC0, 0xFFFFFFFFFFFFFF80, 0xFFFFFFFFFFFFFF00,
	0xFFFFFFFFFFFFFE00, 0xFFFFFFFFFFFFFC00, 0xFFFFFFFFFFFFF800, 0xFFFFFFFFFFFFF000,
	0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFC000, 0xFFFFFFFFFFFF8000, 0xFFFFFFFFFFFF0000,
	0xFFFFFFFFFFFE0000, 0xFFFFFFFFFFFC0000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFF00000,
	0xFFFFFFFFFFE00000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFF800000, 0xFFFFFFFFFF000000,
	0xFFFFFFFFFE000000, 0xFFFFFFFFFC000000, 0xFFFFFFFFF8000000, 0xFFFFFFFFF0000000,
	0xFFFFFFFFE0000000, 0xFFFFFFFFC0000000, 0xFFFFFFFF80000000, 0xFFFFFFFF00000000,
	0xFFFFFFFE00000000, 0xFFFFFFFC00000000, 0xFFFFFFF800000000, 0xFFFFFFF000000000,
	0xFFFFFFE000000000, 0xFFFFFFC000000000, 0xFFFFFF8000000000, 0xFFFFFF0000000000,
	0xFFFFFE0000000000, 0xFFFFFC0000000000, 0xFFFFF80000000000, 0xFFFFF00000000000,
	0xFFFFE00000000000, 0xFFFFC00000000000, 0xFFFF800000000000, 0xFFFF000000000000,
	0xFFFE000000000000, 0xFFFC000000000000, 0xFFF8000000000000, 0xFFF0000000000000,
	0xFFE0000000000000, 0xFFC0000000000000, 0xFF80000000000000, 0xFF00000000000000,
	0xFE00000000000000, 0xFC00000000000000, 0xF800000000000000, 0xF000000000000000,
	0xE000000000000000, 0xC000000000000000, 0x8000000000000000, 0xFFFFFFFFFFFFFFFF,
	0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFF8, 0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFFE0, 0xFFFFFFFFFFFFFFC0, 0xFFFFFFFFFFFFFF80, 0xFFFFFFFFFFFFFF00,
	0xFFFFFFFFFFFFFE00, 0xFFFFFFFFFFFFFC00, 0xFFFFFFFFFFFFF800, 0xFFFFFFFFFFFFF000,
	0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFC000, 0xFFFFFFFFFFFF8000, 0xFFFFFFFFFFFF0000,
	0xFFFFFFFFFFFE0000, 0xFFFFFFFFFFFC0000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFF00000,
	0xFFFFFFFFFFE00000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFF800000, 0xFFFFFFFFFF000000,
	0xFFFFFFFFFE000000, 0xFFFFFFFFFC000000, 0xFFFFFFFFF8000000, 0xFFFFFFFFF0000000,
	0xFFFFFFFFE0000000, 0xFFFFFFFFC0000000, 0xFFFFFFFF80000000, 0xFFFFFFFF00000000,
	0xFFFFFFFE00000000, 0xFFFFFFFC00000000, 0xFFFFFFF800000000, 0xFFFFFFF000000000,
	0xFFFFFFE000000000, 0xFFFFFFC000000000, 0xFFFFFF8000000000, 0xFFFFFF0000000000,
	0xFFFFFE0000000000, 0xFFFFFC0000000000, 0xFFFFF80000000000, 0xFFFFF00000000000,
	0xFFFFE00000000000, 0xFFFFC00000000000, 0xFFFF800000000000, 0xFFFF000000000000,
	0xFFFE000000000000, 0xFFFC000000000000, 0xFFF8000000000000, 0xFFF0000000000000,
	0xFFE0000000000000, 0xFFC0000000000000, 0xFF80000000000000, 0xFF00000000000000,
	0xFE00000000000000, 0xFC00000000000000, 0xF800000000000000, 0xF000000000000000,
	0xE000000000000000, 0xC000000000000000, 0x8000000000000000, 0x0000000000000000
};

static const unsigned long long __oneshifted[MASKSZ] = {
	0x0000000000000001,
	0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010,
	0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100,
	0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000,
	0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000,
	0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000,
	0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000,
	0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000,
	0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000,
	0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000,
	0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000,
	0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000,
	0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000,
	0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000,
	0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000,
	0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000,
	0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000001,
	0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010,
	0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100,
	0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000,
	0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000,
	0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000,
	0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000,
	0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000,
	0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000,
	0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000,
	0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000,
	0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000,
	0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000,
	0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000,
	0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000,
	0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000,
	0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000000
};

static const char __index[MASKSZ] = {
	0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 2
};

/*
 * Returns the number of 1-bits in x.
 */
static inline int __builtin_popcountsse(NODETYPE x) {
	return __builtin_popcountll(x.ui64[0]) + __builtin_popcountll(x.ui64[1]);
}

/*
 * Returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero.
 */
static inline int __builtin_ffssse(NODETYPE x) {
	if (x.ui64[0] != 0)
		return __builtin_ffsll(x.ui64[0]);
	if (x.ui64[1] != 0)
		return __builtin_ffsll(x.ui64[1]) + 64;
	return 0;
}

/*
 * Returns one plus the index of the significant 1-bit of x next to i, or if x is zero, returns zero.
 */
static inline int __builtin_nextffssse(NODETYPE x, int i) {
	unsigned long long int masked;
	i++;
	if (i < 64)
	{
		masked = x.ui64[0] & __mask[i];
		if (masked != 0)
			return __builtin_ffsll(masked);
		i = 64;
	}
	masked = x.ui64[1] & __mask[i];
	if (masked == 0)
		return 0;
	return __builtin_ffsll(masked) + 64;
}

/*
 * Returns the number of leading 0-bits in x, starting at the most significant bit position. If x is 0, the result is undefined.
 */
static inline int __builtin_clzsse(NODETYPE x) {
	if (x.ui64[1] != 0)
		return __builtin_clzll(x.ui64[1]);
	return __builtin_clzll(x.ui64[0]) + 64;
}

/*
 * Returns the number of leading 0-bits in x, starting at ith-bit position. If x is 0, the result is BSIZE.
 */
static inline int __builtin_prevclzsse(NODETYPE x, int i) {
	unsigned long long int masked;
	if (i > 64)
	{
		masked = x.ui64[1] & ~__mask[i];
		if (masked != 0)
			return __builtin_clzll(masked);
		i = 64;
	}
	masked = x.ui64[0] & ~__mask[i];
	if (masked == 0)
		return BSIZE;
	return __builtin_clzll(masked) + 64;
}

#define NODECOUNTER(i) __builtin_popcountsse(i)
#define NODELEAST(w) (__builtin_ffssse(w)-1)
#define NODENEXTLEAST(w, i) (__builtin_nextffssse((w), (i))-1)
#define NODEMOST(i) (BSIZE-__builtin_clzsse(i)-1)
#define NODEPREVMOST(w, i) (BSIZE-__builtin_prevclzsse((w), (i))-1)
#define NODEISEMPTY(w) ((w).ui64[0] == 0 && (w).ui64[1] == 0)
#define NODEISNOTEMPTY(w) ((w).ui64[0] != 0 || (w).ui64[1] != 0)
#define ISINNODE(w, i) ((w) & __oneshifted[i])							//!< Returns a node set to a non-zero value if the bit of index \c i in node \c w is a 1-bit; otherwise a node set to \c 0.

#elif _ARCH32_  // see comments at #ifdef _ARCH8_
typedef unsigned long int NODETYPE; //!< Bitmap node's type
#define BSIZE 32					//!< Number of bits in a node
#define WSIZE 4						//!< Number of bytes in a node
#define LOGB 5L						//!< Logarithmic of BSIZE
#define LOGW 2L						//!< Logarithmic of WSIZE
#define LOGBW 7L					//!< LOGB \c + LOGW
#define ONE 1L						//!< Node set to \c 1
#define NODEMAX 0xFFFFFFFF			//!< Node's maximum value
#define MASKB 0x1FL					//!< Mask used to select the LOGB least significant bits
//#define MASKB 0xFFFFFFE0

#define MASKSZ (BSIZE+1)			//!< Number of elements in __mask
static const NODETYPE __mask[MASKSZ] = {
	0xFFFFFFFF,
	0xFFFFFFFE, 0xFFFFFFFC, 0xFFFFFFF8, 0xFFFFFFF0,
	0xFFFFFFE0, 0xFFFFFFC0, 0xFFFFFF80, 0xFFFFFF00,
	0xFFFFFE00, 0xFFFFFC00, 0xFFFFF800, 0xFFFFF000,
	0xFFFFE000, 0xFFFFC000, 0xFFFF8000, 0xFFFF0000,
	0xFFFE0000, 0xFFFC0000, 0xFFF80000, 0xFFF00000,
	0xFFE00000, 0xFFC00000, 0xFF800000, 0xFF000000,
	0xFE000000, 0xFC000000, 0xF8000000, 0xF0000000,
	0xE0000000, 0xC0000000, 0x80000000, 0x00000000
};

static const NODETYPE __oneshifted[MASKSZ] = {
	0x00000001,
	0x00000002, 0x00000004, 0x00000008, 0x00000010,
	0x00000020, 0x00000040, 0x00000080, 0x00000100,
	0x00000200, 0x00000400, 0x00000800, 0x00001000,
	0x00002000, 0x00004000, 0x00008000, 0x00010000,
	0x00020000, 0x00040000, 0x00080000, 0x00100000,
	0x00200000, 0x00400000, 0x00800000, 0x01000000,
	0x02000000, 0x04000000, 0x08000000, 0x10000000,
	0x20000000, 0x40000000, 0x80000000, 0x00000000
};

static inline int nodeMost(NODETYPE i) {
	return (i) == 0 ? -1 : BSIZE-__builtin_clzl(i)-1;
}

#define NODECOUNTER(i) __builtin_popcountl(i)
#define NODELEAST(w) (__builtin_ffsl(w)-1)
#define NODENEXTLEAST(w, i) (NODELEAST((w) & __mask[i + 1]))
#define NODEMOST(i) (nodeMost(i))
#define NODEPREVMOST(w, i) (NODEMOST((w) & ~__mask[i]))
#define NODEISEMPTY(w) (w == 0)
#define NODEISNOTEMPTY(w) (w != 0)
#define ISINNODE(w, i) ((w) & (ONE << (i)))							//!< Returns a node set to a non-zero value if the bit of index \c i in node \c w is a 1-bit; otherwise a node set to \c 0.

#elif _ARCH64_

typedef unsigned long long int NODETYPE; //!< bitmap node's type
#define BSIZE 64
#define WSIZE 8
#define LOGB 6LL
#define LOGW 3
#define LOGBW 9

#define ONE 1LL
#define NODEMAX 0xFFFFFFFFFFFFFFFF
#define MASKB 0x3FLL
//#define MASKB 0xFFFFFFFFFFFFFFC0

#define MASKSZ (BSIZE+1)
static const NODETYPE __mask[MASKSZ] = {
	0xFFFFFFFFFFFFFFFF,
	0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFF8, 0xFFFFFFFFFFFFFFF0,
	0xFFFFFFFFFFFFFFE0, 0xFFFFFFFFFFFFFFC0, 0xFFFFFFFFFFFFFF80, 0xFFFFFFFFFFFFFF00,
	0xFFFFFFFFFFFFFE00, 0xFFFFFFFFFFFFFC00, 0xFFFFFFFFFFFFF800, 0xFFFFFFFFFFFFF000,
	0xFFFFFFFFFFFFE000, 0xFFFFFFFFFFFFC000, 0xFFFFFFFFFFFF8000, 0xFFFFFFFFFFFF0000,
	0xFFFFFFFFFFFE0000, 0xFFFFFFFFFFFC0000, 0xFFFFFFFFFFF80000, 0xFFFFFFFFFFF00000,
	0xFFFFFFFFFFE00000, 0xFFFFFFFFFFC00000, 0xFFFFFFFFFF800000, 0xFFFFFFFFFF000000,
	0xFFFFFFFFFE000000, 0xFFFFFFFFFC000000, 0xFFFFFFFFF8000000, 0xFFFFFFFFF0000000,
	0xFFFFFFFFE0000000, 0xFFFFFFFFC0000000, 0xFFFFFFFF80000000, 0xFFFFFFFF00000000,
	0xFFFFFFFE00000000, 0xFFFFFFFC00000000, 0xFFFFFFF800000000, 0xFFFFFFF000000000,
	0xFFFFFFE000000000, 0xFFFFFFC000000000, 0xFFFFFF8000000000, 0xFFFFFF0000000000,
	0xFFFFFE0000000000, 0xFFFFFC0000000000, 0xFFFFF80000000000, 0xFFFFF00000000000,
	0xFFFFE00000000000, 0xFFFFC00000000000, 0xFFFF800000000000, 0xFFFF000000000000,
	0xFFFE000000000000, 0xFFFC000000000000, 0xFFF8000000000000, 0xFFF0000000000000,
	0xFFE0000000000000, 0xFFC0000000000000, 0xFF80000000000000, 0xFF00000000000000,
	0xFE00000000000000, 0xFC00000000000000, 0xF800000000000000, 0xF000000000000000,
	0xE000000000000000, 0xC000000000000000, 0x8000000000000000, 0x0000000000000000
};

static const NODETYPE __oneshifted[MASKSZ] = {
	0x0000000000000001,
	0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010,
	0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000100,
	0x0000000000000200, 0x0000000000000400, 0x0000000000000800, 0x0000000000001000,
	0x0000000000002000, 0x0000000000004000, 0x0000000000008000, 0x0000000000010000,
	0x0000000000020000, 0x0000000000040000, 0x0000000000080000, 0x0000000000100000,
	0x0000000000200000, 0x0000000000400000, 0x0000000000800000, 0x0000000001000000,
	0x0000000002000000, 0x0000000004000000, 0x0000000008000000, 0x0000000010000000,
	0x0000000020000000, 0x0000000040000000, 0x0000000080000000, 0x0000000100000000,
	0x0000000200000000, 0x0000000400000000, 0x0000000800000000, 0x0000001000000000,
	0x0000002000000000, 0x0000004000000000, 0x0000008000000000, 0x0000010000000000,
	0x0000020000000000, 0x0000040000000000, 0x0000080000000000, 0x0000100000000000,
	0x0000200000000000, 0x0000400000000000, 0x0000800000000000, 0x0001000000000000,
	0x0002000000000000, 0x0004000000000000, 0x0008000000000000, 0x0010000000000000,
	0x0020000000000000, 0x0040000000000000, 0x0080000000000000, 0x0100000000000000,
	0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000,
	0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000000
};

static inline int nodeMost(NODETYPE i) {
	return (i) == 0 ? -1 : BSIZE-__builtin_clzll(i)-1;
}

#define NODECOUNTER(i) __builtin_popcountll(i)
#define NODELEAST(w) (__builtin_ffsll(w)-1)
#define NODENEXTLEAST(w, i) (NODELEAST((w) & __mask[i + 1]))
#define NODEMOST(i) (nodeMost(i))
#define NODEPREVMOST(w, i) (NODEMOST((w) & ~__mask[i]))
#define NODEISEMPTY(w) (w == 0)
#define NODEISNOTEMPTY(w) (w != 0)
#define ISINNODE(w, i) ((w) & (ONE << (i)))							//!< Returns a node set to a non-zero value if the bit of index \c i in node \c w is a 1-bit; otherwise a node set to \c 0.

#else
/*! \typedef NODETYPE
 * \brief Bitmap node's type
 *
 */
typedef unsigned char NODETYPE; 			//!< Bitmap node's type
#define BSIZE 8								//!< Number of bits in a node
#define WSIZE 1								//!< Number of bytes in a node
#define LOGB 3								//!< Logarithmic of BSIZE
#define LOGW 0								//!< Logarithmic of WSIZE
#define LOGBW 3								//!< LOGB \c + LOGW
#define ONE 1								//!< Node set to \c 1
#define NODEMAX 0xFF						//!< Node's maximum value
#define MASKB 0x07							//!< Mask used to select the LOGB least significant bits
//#define MASKB 0xFFFFFFE0

#define MASKSZ (BSIZE+1)					//!< Number of elements in __mask
static const NODETYPE __mask[MASKSZ] = { //!< Array of masks. Entry \c i deletes the \c i least significant bits.
		0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00 };

static inline int nodeMost(NODETYPE i) {
	return (i) == 0 ? -1 : BSIZE-__builtin_clz(i)-1;
}

#define NODECOUNTER(w) __builtin_popcount(w)						//!< Builtin function: Returns the number of 1-bits in \c w.
#define NODELEAST(w) (__builtin_ffs(w)-1)							//!< Builtin function: Returns the index of the least significant 1-bit of \c w, or if \c w is zero, returns \c -1.
#define NODENEXTLEAST(w, i) (NODELEAST((w) & __mask[i + 1]))		//!< Builtin function: Returns the index of the least significant 1-bit of \c w greater than \c i, or if \c w is zero, returns \c -1.
#define NODEMOST(i) (nodeMost(i))									//!< Builtin function: Returns the index of the most significant 1-bit of \c w, or if \c w is zero, returns \c -1.
#define NODEPREVMOST(w, i) (NODEMOST((w) & ~__mask[i]))				//!< Builtin function: Returns the index of the most significant 1-bit of \c w smaller than \c i, or if \c w is zero, returns \c -1.
#define NODEISEMPTY(w) (w == 0)
#define NODEISNOTEMPTY(w) (w != 0)
#define ISINNODE(w, i) ((w) & (ONE << (i)))							//!< Returns a node set to a non-zero value if the bit of index \c i in node \c w is a 1-bit; otherwise a node set to \c 0.
#endif

#define NELEM(i) ((i) << LOGB)										//!< Smallest element in the entry \c i of the bitmap.
#define NODEIDX(i) ((i) >> LOGB)									//!< Node index in the bitmap of element \c i.
#define IDXINNODE(i) ((i) & MASKB)									//!< Element index in the corresponding node of the bitmap.
#define RETAINLEAST(w, i) ((w) & (NODEMAX >> (BSIZE-(i)))) 			//!< Retains \c i least significant elements of node \c w.

typedef struct {
	const int n;
	const int size;
	const int step;  // in words
	const int nRnodes;
	const char * ptr;		//!< Unaligned pointer to this map
	NODETYPE * S;	//!< Summary
	NODETYPE * R;	//!< Map
} BitMap;

typedef struct {
//	int indS;			//!< Node index in a summary.
	int ind;			//!< Node index in a bitmap.
	int nelem;			//!< Smallest element that can be in this node of a bitmap.
	int cur;			//!< Current element as an index in this node.
	NODETYPE * begS;	//!< Used for backward enumeration
	NODETYPE * endS;	//!< Used for forward enumeration
	NODETYPE * bufR;	//!< Used to enumerate
	NODETYPE * bufS;	//!< Used to enumerate
} Node;

static void printNode(Node * node);
static void printSetNodes(BitMap const * s);

static inline int inline_ceillog2(int n) {
	int c = 0;
	int l = 0;
	while (n > 1) {
		c |= (n & 1);
		n >>= 1;
		l++;
	}
	l += c;
	return l;
}

static inline BitMap * const newBitMapVector(size_t size, size_t n) {
	int tamR = NODEIDX(n-1) + 1;
	int tamS = NODEIDX(tamR-1) + 1;
	int step = inline_ceillog2(tamR+tamS);
	int tamRS = 1 << step;
	int tam = (tamRS<<LOGW);

	BitMap * ret = (BitMap *) malloc(WSIZE+WSIZE+sizeof(BitMap)+tam*size);
	if (ret==NULL)
		return(NULL);

	BitMap aux = { n, size, step, tamR, (char *) ret, NULL, NULL };
	ret =  (BitMap *) (((char *) ret) + (WSIZE - (((intptr_t) ret) & (WSIZE-1))));
	memcpy(ret, &aux, sizeof(BitMap));

	char * maps_ptr = ((char *) ret) + sizeof(BitMap);
	ret->S = (NODETYPE *) (maps_ptr + (WSIZE - (((intptr_t) maps_ptr) & (WSIZE-1))));
	ret->R = ret->S + tamS;

	int i, s;
	for (i = 0, s = 0; i < size; i++, s += tamRS)
		memset(ret->S + s, 0, tamS<<LOGW);

	return ret;
}

static inline BitMap * const newBitMap(size_t n) {
	return newBitMapVector(1, n);
}

static inline BitMap *newViewOf(BitMap * const bm) {
	BitMap * ret = (BitMap *) malloc(WSIZE+sizeof(BitMap));
	if (ret==NULL)
		return(NULL);

	ret =  (BitMap *) (((char *) ret) + (WSIZE - (((intptr_t) ret) & (WSIZE-1))));
	memcpy(ret, bm, sizeof(BitMap));
	ret->ptr = (char *) ret;
	memset((int *) &ret->size, 1, sizeof(int));

	return ret;
}

static inline void setAsViewOf(BitMap * const bm, BitMap * v) {
	const char * aux = v->ptr;
	memcpy(v, bm, sizeof(BitMap));
	v->ptr = aux;
	memset((int *) &v->size, 1, sizeof(int));
}

static inline void freeBitMap(BitMap * const ptr) {
	free((char *) ptr->ptr);
}

static inline void subMap(BitMap * const bm, BitMap * r, int i) {
	int is = (i<<bm->step);
	r->S = bm->S + is;
	r->R = bm->R + is;
}

// Deletes bit \c i from node \c w.
static inline void delElement(BitMap * bm, Node const * i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->R+i->ind) &= ~(ONE << (i->cur));
	if (*(bm->R+i->ind) == 0)
		*(bm->S+NODEIDX(i->ind)) &= ~(ONE << (IDXINNODE(i->ind)));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] &= ~__oneshifted[i->cur];
	if ((bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] == 0)
		(bm->S+NODEIDX(i->ind))->ui64[(unsigned int) __index[IDXINNODE(i->ind)]] &= ~__oneshifted[IDXINNODE(i->ind)];
#endif
}

// Adds bit \c i to node \c w.
static inline void addElement(BitMap * bm, Node const * i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	if ((*(bm->S+NODEIDX(i->ind)) & (ONE << (IDXINNODE(i->ind)))) == 0) {
		*(bm->S+NODEIDX(i->ind)) |= (ONE << (IDXINNODE(i->ind)));
		*(bm->R+i->ind) = (ONE << (i->cur));
	}
	else
		*(bm->R+i->ind) |= (ONE << (i->cur));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	if (((bm->S+NODEIDX(i->ind))->ui64[(unsigned int) __index[IDXINNODE(i->ind)]] & __oneshifted[IDXINNODE(i->ind)]) == 0) {
		(bm->S+NODEIDX(i->ind))->ui64[(unsigned int) __index[IDXINNODE(i->ind)]] |= __oneshifted[IDXINNODE(i->ind)];
		(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] = __oneshifted[i->cur];
	}
	else
		(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] |= __oneshifted[i->cur];
#endif
}

// Inverts bit \c i to node \c w.
static inline void invElement(BitMap * bm, Node const * i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	if ((*(bm->S+NODEIDX(i->ind)) & (ONE << (IDXINNODE(i->ind)))) == 0) {
		*(bm->S+NODEIDX(i->ind)) |= (ONE << (IDXINNODE(i->ind)));
		*(bm->R+i->ind) = (ONE << (i->cur));
	}
	else {
		*(bm->R+i->ind) ^= (ONE << (i->cur));
		if (*(bm->R+i->ind) == 0)
			*(bm->S+NODEIDX(i->ind)) &= ~(ONE << (IDXINNODE(i->ind)));
	}
#elif defined(_ARCH128_) || defined(_ARCH256_)
	if (((bm->S+NODEIDX(i->ind))->ui64[(unsigned int) __index[IDXINNODE(i->ind)]] & __oneshifted[IDXINNODE(i->ind)]) == 0) {
		(bm->S+NODEIDX(i->ind))->ui64[(unsigned int) __index[IDXINNODE(i->ind)]] |= __oneshifted[IDXINNODE(i->ind)];
		(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] = __oneshifted[i->cur];
	}
	else {
		(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] ^= __oneshifted[i->cur];
		if ((bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] == 0)
			(bm->S+NODEIDX(i->ind))->ui64[(unsigned int) __index[IDXINNODE(i->ind)]] &= ~__oneshifted[IDXINNODE(i->ind)];
	}
#endif
}

//!< Returns a node set to a non-zero value if the element \c i is in the bitmap \c T; otherwise a node set to \c 0.
static inline int hasElement(BitMap const * bm, int i) {
	int nodeidx = NODEIDX(i);
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	return ISINNODE(*(bm->R+nodeidx), IDXINNODE(i)) != 0 && ISINNODE(*(bm->S+NODEIDX(nodeidx)), IDXINNODE(nodeidx)) != 0;
#elif defined(_ARCH128_) || defined(_ARCH256_)
	int idx = IDXINNODE(i);
	if (ISINNODE((bm->R+nodeidx)->ui64[(unsigned int) __index[idx]], idx) != 0)
	{
		idx = IDXINNODE(nodeidx);
		ISINNODE((bm->S+NODEIDX(nodeidx))->ui64[(unsigned int) __index[idx]], idx) != 0;
	}
	return 0;
#endif
}

static inline int isEmpty(BitMap * mvec) {
	NODETYPE * node = mvec->S;
	while (node < mvec->R)
	{
		if (NODEISNOTEMPTY(*node))
			return 0;
		node++;
	}
	return 1;
}

static inline int isNotEmpty(BitMap * mvec) {
	NODETYPE * node = mvec->S;
	while (node < mvec->R)
	{
		if (NODEISNOTEMPTY(*node))
			return 1;
		node++;
	}
	return 0;
}

// forward enumeration

static inline void beginWhile(BitMap const * bm, Node * node) {
//	node->indS = 0;
	node->ind = 0;
	node->nelem = 0;
	node->cur = -1;
	node->begS = bm->S;
	node->endS = bm->R;
	node->bufR = bm->R;
	node->bufS = bm->S;
}

static inline void nextCommon(BitMap const * bm, Node * node) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	if ((*node->bufS & (ONE << IDXINNODE(node->ind))) > 0)
		node->cur = NODENEXTLEAST(*node->bufR & *(bm->R+node->ind), node->cur);
	else
		node->cur = -1;
	if (node->cur < 0 && node->bufS < node->endS) {
		int i = IDXINNODE(node->ind);
		int j = NODEIDX(node->ind);
		NODETYPE * bnodeS = bm->S+j;
		NODETYPE * R = node->bufR - node->ind;
		NODETYPE bufS = *node->bufS & *bnodeS;
		while (node->cur < 0 && node->bufS < node->endS) {
			i = NODENEXTLEAST(bufS, i);
			while (i < 0 && ++node->bufS < node->endS) {
				j++;
				bufS = *node->bufS & *++bnodeS;
				i = NODENEXTLEAST(bufS, -1);
			}
			if (node->bufS < node->endS)
			{
				node->ind = ((j << LOGB) + i);
				node->bufR = R + node->ind;
				node->nelem = (node->ind << LOGB);
				node->cur = NODENEXTLEAST(*node->bufR & *(bm->R+node->ind), -1);
			}
		}
	}
#elif defined(_ARCH128_)
	NODETYPE bufR;
	if ((*node->bufS & (ONE << IDXINNODE(node->ind))) > 0) {
		_mm_store_si128 (&bufR.mi, _mm_and_si128 ((bm->R+node->ind)->mi, node->bufR->mi));
		node->cur = NODENEXTLEAST(bufR, node->cur);
	}
	else
		node->cur = -1;
	if (node->cur < 0 && node->bufS < node->endS) {
		int i = IDXINNODE(node->ind);
		int j = NODEIDX(node->ind);
		NODETYPE * bnodeS = bm->S+j;
		NODETYPE * R = node->bufR - node->ind;
		NODETYPE bufS;
	    _mm_store_si128 (&bufS.mi, _mm_and_si128 (bnodeS->mi, node->bufS->mi));
		while (node->cur < 0 && node->bufS < node->endS) {
			i = NODENEXTLEAST(bufS, i);
			while (i < 0 && ++node->bufS < node->endS) {
				j++;
			    _mm_store_si128 (&bufS.mi, _mm_and_si128 ((++bnodeS)->mi, node->bufS->mi));
				i = NODENEXTLEAST(bufS, -1);
			}
			if (node->bufS < node->endS)
			{
				node->ind = ((j << LOGB) + i);
				node->bufR = R + node->ind;
				node->nelem = (node->ind << LOGB);
			    _mm_store_si128 (&bufR.mi, _mm_and_si128 ((bm->R+node->ind)->mi, node->bufR->mi));
				node->cur = NODENEXTLEAST(bufR, -1);
			}
		}
	}
#elif defined(_ARCH256_)
	NODETYPE bufR;
	if ((*node->bufS & (ONE << IDXINNODE(node->ind))) > 0) {
		_mm256_store_pd(&bufR.d64,_mm256_and_pd((bm->R+node->ind)->md, node->bufR->md));
		node->cur = NODENEXTLEAST(bufR, node->cur);
	}
	else
		node->cur = -1;
	if (node->cur < 0 && node->bufS < node->endS) {
		int i = IDXINNODE(node->ind);
		int j = NODEIDX(node->ind);
		NODETYPE * bnodeS = bm->S+j;
		NODETYPE * R = node->bufR - node->ind;
		NODETYPE bufS;
	    _mm256_store_pd(&bufS.d64,_mm256_and_pd(bnodeS->md, node->bufS->md));
		while (node->cur < 0 && node->bufS < node->endS) {
			i = NODENEXTLEAST(bufS, i);
			while (i < 0 && ++node->bufS < node->endS) {
				j++;
			    _mm256_store_pd(&bufS.d64,_mm256_and_pd((++bnodeS)->md, node->bufS->md));
				i = NODENEXTLEAST(bufS, -1);
			}
			if (node->bufS < node->endS)
			{
				node->ind = ((j << LOGB) + i);
				node->bufR = R + node->ind;
				node->nelem = (node->ind << LOGB);
			    _mm256_store_pd(&bufR.d64,_mm256_and_pd((bm->R+node->ind)->md, node->bufR->md));
				node->cur = NODENEXTLEAST(bufR, -1);
			}
		}
	}
#endif
}

static inline void beginCommon(BitMap const * abm, BitMap const * bm, Node * node) {
	beginWhile(bm, node);
	nextCommon(abm, node);
}

static inline void next(Node * node) {
	if ((*node->bufS & (ONE << IDXINNODE(node->ind))) > 0)
		node->cur = NODENEXTLEAST(*node->bufR, node->cur);
	else
		node->cur = -1;
	if (node->cur < 0 && node->bufS < node->endS) {
		int i = IDXINNODE(node->ind);
		int j = NODEIDX(node->ind);
		NODETYPE * R = node->bufR - node->ind;
		while (node->cur < 0 && node->bufS < node->endS) {
			i = NODENEXTLEAST(*node->bufS, i);
			while (i < 0 && ++node->bufS < node->endS) {
				j++;
				i = NODENEXTLEAST(*node->bufS, -1);
			}
			if (node->bufS < node->endS) {
				node->ind = ((j << LOGB) + i);
				node->bufR = R + node->ind;
				node->nelem = (node->ind << LOGB);
				node->cur = NODENEXTLEAST(*node->bufR, -1);
			}
		}
	}
}

static inline void prev(Node * node) {
	if ((*node->bufS & (ONE << IDXINNODE(node->ind))) > 0)
		node->cur = NODEPREVMOST(*node->bufR, node->cur);
	else
		node->cur = -1;
	if (node->cur < 0 && node->bufS >= node->begS) {
		int i = IDXINNODE(node->ind);
		int j = NODEIDX(node->ind);
		NODETYPE * R = node->bufR - node->ind;
		while (node->cur < 0 && node->bufS >= node->begS) {
			i = NODEPREVMOST(*node->bufS, i);
			while (i < 0 && --node->bufS >= node->begS) {
				j--;
				i = NODEPREVMOST(*node->bufS, BSIZE);
			}
			if (node->bufS >= node->endS) {
				node->ind = ((j << LOGB) + i);
				node->bufR = R + node->ind;
				node->nelem = (node->ind << LOGB);
				node->cur = NODEPREVMOST(*node->bufR, BSIZE);
			}
		}
	}
}

static inline void begin(BitMap const * bm, Node * node) {
	beginWhile(bm, node);
	next(node);
}

static inline int end(Node * node) {
	return node->bufS >= node->endS;
}

static inline void rearWhile(BitMap const * bm, Node * node) {
	node->ind = bm->nRnodes-1;
	node->nelem = NELEM(node->ind);
	node->cur = BSIZE;
	node->begS = bm->S;
	node->endS = bm->R;
	node->bufR = bm->R+node->ind;
	node->bufS = bm->S+NODEIDX(node->ind);
}

static inline void rear(BitMap const * bm, Node * node) {
	rearWhile(bm, node);
	prev(node);
}

static inline int head(Node * node) {
	return node->bufS < node->begS;
}

static inline void getNode(Node * node, int i) {
	node->ind = NODEIDX(i);
//	node->indS = NODEIDX(node->ind);
	node->nelem = NELEM(node->ind);
	node->cur = IDXINNODE(i);
}

static inline void printNode(Node * node) {
	printf("indS=%d  ind=%d  nelem=%d  cur=%d  elem=%d\n",NODEIDX(node->ind), node->ind, node->nelem, node->cur, node->nelem+ node->cur);
}

static inline int getElement(Node const * node) {
	return node->nelem + node->cur;
}

static inline void setElement(Node * node, int i) {
	node->ind = NODEIDX(i);
//	node->indS = NODEIDX(node->ind);
	node->nelem = NELEM(node->ind);
	node->cur = IDXINNODE(i);
}

/*!
 * \fn int cardOf(BitMap * bmvec)
 * \brief Determines the number of elements in the specified nodes of a bitmap.
 *
 * \param bmvec Bitmap.
 *
 * \return The number of elements in the nodes of the bitmap \c bmvec.
 */
static inline int cardOf(BitMap const * bmvec) {
	int ret = 0, i;
	NODETYPE * nodeS;
	NODETYPE * nodeR;
	for (nodeS = bmvec->S, nodeR = bmvec->R; nodeS < bmvec->R; nodeS++, nodeR+=BSIZE)
	{
		i = NODENEXTLEAST(*nodeS, -1);
		while (i >= 0) {
			ret += NODECOUNTER(*(nodeR+i));
			i = NODENEXTLEAST(*nodeS, i);
		}
	}
	return ret;
}

/*!
 * \fn void copyAll(BitMap * bmvec, BitMap * abmvec)
 * \brief Copy nodes between specified bitmaps.
 *
 * \param bmvec Nodes are copied to this bitmap.
 * \param abmvec Nodes are copied from this bitmap.
 */
static inline void copyAll(BitMap * bmvec, BitMap const * abmvec) {
	memcpy(bmvec->S, abmvec->S, ((abmvec->R-abmvec->S)+abmvec->nRnodes) << LOGW);
}

static inline void copySets(Node * node, Node * anode) {
	int i = (anode->bufS-anode->begS);
	node->bufS = node->begS+i;
	node->bufR = node->bufR+(anode->ind-node->ind);
	node->ind = anode->ind;
	node->nelem = anode->nelem;
	node->cur = anode->cur;
	i = (anode->endS-anode->bufS);
	int ii = node->endS-node->bufS;
	if (i > ii)
		i = ii;
	memcpy(node->bufS, anode->bufS, i << LOGW);

	NODETYPE * nodeS;
	NODETYPE * nodeR;
	NODETYPE * anodeR;
	for (nodeS = node->bufS, nodeR = node->bufR-IDXINNODE(node->ind), anodeR = anode->bufR-IDXINNODE(node->ind); nodeS < node->endS;
			nodeS++, nodeR+=BSIZE, anodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		i = NODENEXTLEAST(*nodeS, NODEIDX(node->ind)-1);
		while (i >= 0) {
			*(nodeR+i) = *(anodeR+i);
			i = NODENEXTLEAST(*nodeS, i);
		}
#elif defined(_ARCH128_)
		i = NODENEXTLEAST(*nodeS, NODEIDX(node->ind)-1);
		while (i >= 0) {
			_mm_store_si128 (&(nodeR+i)->mi, (anodeR+i)->mi);
			i = NODENEXTLEAST(*nodeS, i);
		}
#elif defined(_ARCH256_)
		i = NODENEXTLEAST(*nodeS, NODEIDX(node->ind-1));
		while (i >= 0) {
			_mm256_store_pd(&(nodeR+i)->md, (anodeR+i)->md);
			i = NODENEXTLEAST(*nodeS, i);
		}
#endif
	}
}

/*!
 * \fn void addAll(BitMap * bmvec, BitMap * abmvec)
 * \brief Add elements between specified bitmaps.
 *
 * This function performs a logic \c OR operation of the specified nodes.
 *
 * \param bmvec Elements are added to this bitmap.
 * \param abmvec Elements of this bitmap are added.
 */
static inline void addAll(BitMap * bmvec, BitMap const * abmvec) {
	NODETYPE * bnodeS;
	NODETYPE * abnodeS;
	NODETYPE * bnodeR;
	NODETYPE * abnodeR;
	int i;

	for (bnodeS = bmvec->S, abnodeS = abmvec->S, bnodeR = bmvec->R, abnodeR = abmvec->R; abnodeS < abmvec->R;
			bnodeS++, abnodeS++, bnodeR+=BSIZE, abnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		*bnodeS |= *abnodeS;
		i = NODENEXTLEAST(*abnodeS, -1);
		while (i >= 0) {
			*(bnodeR+i) |= *(abnodeR+i);
			i = NODENEXTLEAST(*abnodeS, i);
		}
#elif defined(_ARCH128_)
		_mm_store_si128 (&bnodeS->mi, _mm_or_si128 (bnodeS->mi, abnodeS->mi));
		i = NODENEXTLEAST(*abnodeS, -1);
		while (i >= 0) {
			_mm_store_si128 (&(bnodeR+i)->mi, _mm_or_si128 ((bnodeR+i)->mi, (abnodeR+i)->mi));
			i = NODENEXTLEAST(*abnodeS, i);
		}
#elif defined(_ARCH256_)
		_mm256_store_pd(&bnodeS->md, _mm256_or_pd(bnodeS->md, abnodeS->md));
		i = NODENEXTLEAST(*abnodeS, -1);
		while (i >= 0) {
			_mm256_store_pd(&(bnodeR+i)->md, _mm256_or_pd((bnodeR+i)->md, (abnodeR+i)->md));
			i = NODENEXTLEAST(*abnodeS, i);
		}
#endif
	}
}

static inline void addAllElements(BitMap * bm) {
	memset(bm->S, 0xFF, ((bm->R-bm->S)+bm->nRnodes) << LOGW);

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->S+NODEIDX(bm->nRnodes-1)) = RETAINLEAST(*(bm->S+NODEIDX(bm->nRnodes-1)),IDXINNODE(bm->nRnodes-1)+1);
	*(bm->R+bm->nRnodes-1) = RETAINLEAST(*(bm->R+bm->nRnodes-1),IDXINNODE(bm->n-1)+1);
#elif defined(_ARCH128_) || defined(_ARCH256_)
	int idx = IDXINNODE(bm->nRnodes-1);
	bm->S->ui64[(unsigned int) __index[idx]] = RETAINLEAST(bm->S->ui64[(unsigned int) __index[idx]], idx-((unsigned int) __index[idx] << LOGB)+1);
	idx = IDXINNODE(bm->n-1);
	bm->R->ui64[(unsigned int) __index[idx]] = RETAINLEAST(bm->R->ui64[(unsigned int) __index[idx]], idx-((unsigned int) __index[idx] << LOGB)+1);
#endif
}

//!< Deletes all elements from bitmap \c R.
static inline void delAllElements(BitMap * bm) {
	memset(bm->S, 0, (bm->R-bm->S) << LOGW);
}

/*!
 * \fn void removeAll(BitMap * bmvec, BitMap * abmvec)
 * \brief Remove elements between specified bitmaps.
 *
 * This function performs a logic \c AND operation of the specified nodes from one bitmap with the negation of nodes from the other bitmap.
 *
 * \param bmvec Elements of this bitmap are removed.
 * \param abmvec Elements are removed from this bitmap.
 */
static inline void removeAll(BitMap * bmvec, BitMap const * rbmvec) {
	NODETYPE * bnodeS;
	NODETYPE * rbnodeS;
	NODETYPE * bnodeR;
	NODETYPE * rbnodeR;
	NODETYPE buf;
	int i;

	for (bnodeS = bmvec->S, rbnodeS = rbmvec->S, bnodeR = bmvec->R, rbnodeR = rbmvec->R; bnodeS < bmvec->R;
			bnodeS++, rbnodeS++, bnodeR+=BSIZE, rbnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		buf = *bnodeS & *rbnodeS;
		i = NODENEXTLEAST(buf, -1);
		while (i >= 0) {
			*(bnodeR+i) &= ~*(rbnodeR+i);
			if (*(bnodeR+i) == 0)
				*bnodeS &= ~(ONE << i);
			i = NODENEXTLEAST(buf, i);
		}
#elif defined(_ARCH128_)
		_mm_store_si128 (&buf.mi, _mm_and_si128 (rbnodeS->mi, bnodeS->mi));
		i = NODENEXTLEAST(buf, -1);
		while (i >= 0) {
			_mm_store_si128 (&(bnodeR+i)->mi, _mm_andnot_si128 ((rbnodeR+i)->mi, (bnodeR+i)->mi));
			if (_mm_comieq_ss((bnodeR+i)->mi, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(buf, i);
		}
#elif defined(_ARCH256_)
		_mm256_store_pd(&buf.md,_mm256_and_pd(rbnodeS->md, bnodeS->md));
		i = NODENEXTLEAST(buf, -1);
		while (i >= 0) {
			_mm256_store_pd(&(bnodeR+i)->md,_mm256_andnot_pd((rbnodeR+i)->md, (bnodeR+i)->md));
			if (_mm256_testz_si256((bnodeR+i)->md, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(buf, i);
		}
#endif
	}
}

static inline void removeFrom(Node * node, BitMap const * rbmvec) {
	NODETYPE * bnodeS;
	NODETYPE * rbnodeS;
	NODETYPE * bnodeR;
	NODETYPE * rbnodeR;
	NODETYPE buf;
	int i;

	for (bnodeS = node->bufS, rbnodeS = rbmvec->S+(bnodeS-node->begS), bnodeR = node->bufR-IDXINNODE(node->ind), rbnodeR = rbmvec->R+NODEIDX(node->ind); bnodeS < node->endS;
			bnodeS++, rbnodeS++, bnodeR+=BSIZE, rbnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		buf = *bnodeS & *rbnodeS;
		i = NODENEXTLEAST(buf, NODEIDX(node->ind-1));
		while (i >= 0) {
			*(bnodeR+i) &= ~*(rbnodeR+i);
			if (*(bnodeR+i) == 0)
				*bnodeS &= ~(ONE << i);
			i = NODENEXTLEAST(buf, i);
		}
#elif defined(_ARCH128_)
		_mm_store_si128 (&buf.mi, _mm_and_si128 (rbnodeS->mi, bnodeS->mi));
		i = NODENEXTLEAST(buf, NODEIDX(node->ind-1));
		while (i >= 0) {
			_mm_store_si128 (&(bnodeR+i)->mi, _mm_andnot_si128 ((rbnodeR+i)->mi, (bnodeR+i)->mi));
			if (_mm_comieq_ss((bnodeR+i)->mi, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(buf, i);
		}
#elif defined(_ARCH256_)
		_mm256_store_pd(&buf.md,_mm256_and_pd(rbnodeS->md, bnodeS->md));
		i = NODENEXTLEAST(buf, NODEIDX(node->ind-1));
		while (i >= 0) {
			_mm256_store_pd(&(bnodeR+i)->md,_mm256_andnot_pd((rbnodeR+i)->md, (bnodeR+i)->md));
			if (_mm256_testz_si256((bnodeR+i)->md, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(buf, i);
		}
#endif
	}
}

/*!
 * \fn void retainAll(BitMap * bmvec, BitMap * rbmvec)
 * \brief Intersection of bitmaps with the result in one of them.
 *
 * This function performs a logic \c AND operation of the specified nodes.
 *
 * \param bmvec One input set. Also receives the resulting set.
 * \param rbmvec Another input set.
 */
static inline void retainAll(BitMap * bmvec, BitMap const * rbmvec) {
	NODETYPE * bnodeS;
	NODETYPE * rbnodeS;
	NODETYPE * bnodeR;
	NODETYPE * rbnodeR;
	int i;

	for (bnodeS = bmvec->S, rbnodeS = rbmvec->S, bnodeR = bmvec->R, rbnodeR = rbmvec->R; bnodeS < bmvec->R;
			bnodeS++, rbnodeS++, bnodeR+=BSIZE, rbnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		*bnodeS &= *rbnodeS;
		i = NODENEXTLEAST(*bnodeS, -1);
		while (i >= 0) {
			*(bnodeR+i) &= *(rbnodeR+i);
			if (*(bnodeR+i) == 0)
				*bnodeS &= ~(ONE << i);
			i = NODENEXTLEAST(*bnodeS, i);
		}
#elif defined(_ARCH128_)
		_mm_store_si128 (&bnodeS->mi, _mm_and_si128 (rbnodeS->mi, bnodeS->mi));
		i = NODENEXTLEAST(*bnodeS, -1);
		while (i >= 0) {
			_mm_store_si128 (&(bnodeR+i)->mi, _mm_and_si128 ((rbnodeR+i)->mi, (bnodeR+i)->mi));
			if (_mm_comieq_ss((bnodeR+i)->mi, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(*bnodeS, i);
		}
#elif defined(_ARCH256_)
		_mm256_store_pd(&bnodeS->md,_mm256_and_pd(rbnodeS->md, bnodeS->md));
		i = NODENEXTLEAST(*bnodeS, -1);
		while (i >= 0) {
			_mm256_store_pd(&(bnodeR+i)->md,_mm256_and_pd((rbnodeR+i)->md, (bnodeR+i)->md));
			if (_mm256_testz_si256((bnodeR+i)->md, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(*bnodeS, i);
		}
#endif
	}
}

static inline void retainFrom(BitMap * bmvec, BitMap const * rbmvec, Node * node) {
	NODETYPE * bnodeS;
	NODETYPE * rbnodeS;
	NODETYPE * bnodeR;
	NODETYPE * rbnodeR;
	int i;

	for (bnodeS = node->bufS, rbnodeS = rbmvec->S+(bnodeS-node->begS), bnodeR = node->bufR-IDXINNODE(node->ind), rbnodeR = rbmvec->R+NODEIDX(node->ind); bnodeS < node->endS;
			bnodeS++, rbnodeS++, bnodeR+=BSIZE, rbnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		*bnodeS &= *rbnodeS;
		i = NODENEXTLEAST(*bnodeS, NODEIDX(node->ind-1));
		while (i >= 0) {
			*(bnodeR+i) &= *(rbnodeR+i);
			if (*(bnodeR+i) == 0)
				*bnodeS &= ~(ONE << i);
			i = NODENEXTLEAST(*bnodeS, i);
		}
#elif defined(_ARCH128_)
		_mm_store_si128 (&bnodeS->mi, _mm_and_si128 (rbnodeS->mi, bnodeS->mi));
		i = NODENEXTLEAST(*bnodeS, NODEIDX(node->ind-1));
		while (i >= 0) {
			_mm_store_si128 (&(bnodeR+i)->mi, _mm_and_si128 ((rbnodeR+i)->mi, (bnodeR+i)->mi));
			if (_mm_comieq_ss((bnodeR+i)->mi, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(*bnodeS, i);
		}
#elif defined(_ARCH256_)
		_mm256_store_pd(&bnodeS->md,_mm256_and_pd(rbnodeS->md, bnodeS->md));
		i = NODENEXTLEAST(*bnodeS, NODEIDX(node->ind-1));
		while (i >= 0) {
			_mm256_store_pd(&(bnodeR+i)->md,_mm256_and_pd((rbnodeR+i)->md, (bnodeR+i)->md));
			if (_mm256_testz_si256((bnodeR+i)->md, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(*bnodeS, i);
		}
#endif
	}

}

static inline void symDiff(BitMap * bmvec, BitMap const * rbmvec) {
	NODETYPE * bnodeS;
	NODETYPE * rbnodeS;
	NODETYPE * bnodeR;
	NODETYPE * rbnodeR;
	NODETYPE buf;
	int i;

	for (bnodeS = bmvec->S, rbnodeS = rbmvec->S, bnodeR = bmvec->R, rbnodeR = rbmvec->R; bnodeS < bmvec->R;
			bnodeS++, rbnodeS++, bnodeR+=BSIZE, rbnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		buf = *bnodeS | *rbnodeS;
		i = NODENEXTLEAST(buf, -1);
		while (i >= 0) {
			*(bnodeR+i) ^= *(rbnodeR+i);
			if (*(bnodeR+i) == 0)
				*bnodeS &= ~(ONE << i);
			i = NODENEXTLEAST(buf, i);
		}
#elif defined(_ARCH128_)
		_mm_store_si128 (&buf.mi, _mm_or_si128 (rbnodeS->mi, bnodeS->mi));
		i = NODENEXTLEAST(buf, -1);
		while (i >= 0) {
			_mm_store_si128 (&(bnodeR+i)->mi, _mm_xor_si128 ((rbnodeR+i)->mi, (bnodeR+i)->mi));
			if (_mm_comieq_ss((bnodeR+i)->mi, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(buf, i);
		}
#elif defined(_ARCH256_)
		_mm256_store_pd(&buf.md,_mm256_or_pd(rbnodeS->md, bnodeS->md));
		i = NODENEXTLEAST(buf, -1);
		while (i >= 0) {
			_mm256_store_pd(&(bnodeR+i)->md,_mm256_xor_pd((rbnodeR+i)->md, (bnodeR+i)->md));
			if (_mm256_testz_si256((bnodeR+i)->md, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] &= ~__oneshifted[i];
			i = NODENEXTLEAST(buf, i);
		}
#endif
	}
}

/*!
 * \fn void intersectOf(BitMap * bmvec, BitMap * abmvec, BitMap * bbmvec)
 * \brief Intersection of bitmaps with the result in a third bitmap.
 *
 * This function performs a logic \c AND operation of the specified nodes.
 *
 * \param bmvec Result bitmap
 * \param abmvec Operand bitmap
 * \param bbmvec Operand bitmap
 */
static inline void intersectOf(BitMap * bmvec, BitMap const * abmvec, BitMap const * bbmvec) {
	NODETYPE * bnodeS;
	NODETYPE * abnodeS;
	NODETYPE * bbnodeS;
	NODETYPE * bnodeR;
	NODETYPE * abnodeR;
	NODETYPE * bbnodeR;
	int i;

	for (bnodeS = bmvec->S, abnodeS = abmvec->S, bbnodeS = bbmvec->S, bnodeR = bmvec->R, abnodeR = abmvec->R, bbnodeR = bbmvec->R;
			bnodeS < bmvec->R; bnodeS++, abnodeS++, bbnodeS++, bnodeR+=BSIZE, abnodeR+=BSIZE, bbnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		*bnodeS = *abnodeS & *bbnodeS;
		i = NODENEXTLEAST(*bnodeS, -1);
		while (i >= 0) {
			*(bnodeR+i) = *(bbnodeR+i) & *(abnodeR+i);
			if (*(bnodeR+i) == 0)
				*bnodeS = *bnodeS & ~(ONE << i);
			i = NODENEXTLEAST(*bnodeS, i);
		}
#elif defined(_ARCH128_)
		_mm_store_si128 (&bnodeS->mi, _mm_and_si128 (abnodeS->mi, bbnodeS->mi));
		i = NODENEXTLEAST(*bnodeS, -1);
		while (i >= 0) {
			_mm_store_si128 (&(bnodeR+i)->mi, _mm_and_si128 ((bbnodeR+i)->mi, (abnodeR+i)->mi));
			if (_mm_comieq_ss((bnodeR+i)->mi, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] = bnodeS->ui64[(unsigned int) __index[i]] & ~__oneshifted[i];
			i = NODENEXTLEAST(*bnodeS, -1);
		}
#elif defined(_ARCH256_)
		_mm256_store_pd(&bnodeS->md,_mm256_and_pd(abnodeS->md, bbnodeS->md));
		i = NODENEXTLEAST(*bnodeS, -1);
		while (i >= 0) {
			_mm256_store_pd(&(bnodeR+i)->md,_mm256_and_pd((bbnodeR+i)->md, (abnodeR+i)->md));
			if (_mm256_testz_si256((bnodeR+i)->md, __zero))
				bnodeS->ui64[(unsigned int) __index[i]] = bnodeS->ui64[(unsigned int) __index[i]] & ~__oneshifted[i];
			i = NODENEXTLEAST(*bnodeS, -1);
		}
#endif
	}
}

static inline int isSubset(BitMap const * abmvec, BitMap const * bbmvec) {
	NODETYPE * abnodeS;
	NODETYPE * bbnodeS;
	NODETYPE * abnodeR;
	NODETYPE * bbnodeR;
	int i;

	for (abnodeS = abmvec->S, bbnodeS = bbmvec->S, abnodeR = abmvec->R, bbnodeR = bbmvec->R;
			abnodeS < abmvec->R && *abnodeS == (*abnodeS & *bbnodeS); abnodeS++, bbnodeS++, abnodeR+=BSIZE, bbnodeR+=BSIZE)
	{
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		i = NODENEXTLEAST(*abnodeS, -1);
		while (i >= 0)
			if (*(abnodeR+i) == (*(abnodeR+i) & *(bbnodeR+i)))
				i = NODENEXTLEAST(*abnodeS, i);
			else
				return 0;
#elif defined(_ARCH128_)
		i = NODENEXTLEAST(*abnodeS, -1);
		while (i >= 0)
			if (_mm_comieq_ss((abnodeR+i)->m, (__m128) _mm_and_si128((abnodeR+i)->mi, (bbnodeR+i)->mi)))
				i = NODENEXTLEAST(*abnodeS, i);
			else
				return 0;
#elif defined(_ARCH256_)
		i = NODENEXTLEAST(*abnodeS, -1);
		while (i >= 0)
			if (_mm256_testz_si256((abnodeR+i)->mi, (__m256i) _mm256_and_pd((abnodeR+i)->md, (bbnodeR+i)->md)))
				i = NODENEXTLEAST(*abnodeS, i);
			else
				return 0;
#endif
	}
	return abnodeS == abmvec->R;
}

static inline void printSetNodes(BitMap const * s) {
	NODETYPE * node;

#ifdef _ARCH32_
#define FORMX " %lx"
#elif _ARCH64_
#define FORMX " %llx"
#elif _ARCH128_
#define FORMX " %llx %llx"
#elif _ARCH256_
#define FORMX " %llx %llx %llx %llx"
#else
#define FORMX " %x"
#endif

	printf("SUMMARY: [");
	fflush(NULL);
	for (node = s->R-1; node >= s->S; node--)
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		printf(FORMX, *node);
#elif defined(_ARCHA128_)
		printf(FORMX, node->ui64[1], node->ui64[0]);
#elif defined(_ARCH256_)
		printf(FORMX, node->ui64[3], node->ui64[2], node->ui64[1], node->ui64[0]);
#endif
	printf(" ]  ");
	fflush(NULL);
	printf("SET: [");
	fflush(NULL);
	for (node = s->R+s->nRnodes-1; node >= s->R; node--)
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		printf(FORMX, *node);
#elif defined(_ARCH128_)
		printf(FORMX, node->ui64[1], node->ui64[0]);
#elif defined(_ARCH256_)
		printf(FORMX, node->ui64[3], node->ui64[2], node->ui64[1], node->ui64[0]);
#endif
	printf(" ]\n");
	fflush(NULL);
}

#endif /* BITMAP_H_ */
