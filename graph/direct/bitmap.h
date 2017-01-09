/*!
 * \file direct/bitmap.h
 *
 * \date Sep 24, 2013
 * \author Ricardo C. Corrêa (correa@ufrrj.br)
 *
 * \brief Macros and inline functions for bitwise operations on bitmaps using bit-parallelism.
 *
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
    double d64;
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
	if (i < 192)
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
		i = 192;
	}
	if (i > 128)
	{
		if (i < 192)
			masked = x.ui64[2] & ~__mask[i];
		else
			masked = x.ui64[2];
		if (masked != 0)
			return __builtin_clzll(masked) + 64;
		i = 128;
	}
	if (i > 64)
	{
		if (i < 128)
			masked = x.ui64[1] & ~__mask[i];
		else
			masked = x.ui64[1];
		if (masked != 0)
			return __builtin_clzll(masked) + 128;
		i = 64;
	}
	if (i < 64)
		masked = x.ui64[0] & ~__mask[i];
	else
		masked = x.ui64[0];
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
	if (x.ui64[0] != 0)
		return __builtin_clzll(x.ui64[0]) + 192;
	return BSIZE;
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
 * Returns the number of leading 0-bits in x, starting at the most significant bit position. If x is 0, the result is BSIZE.
 */
static inline int __builtin_clzsse(NODETYPE x) {
	if (x.ui64[1] != 0)
		return __builtin_clzll(x.ui64[1]);
	if (x.ui64[0] != 0)
		return __builtin_clzll(x.ui64[0]) + 64;
	return BSIZE;
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
	if (i < 64)
		masked = x.ui64[0] & ~__mask[i];
	else
		masked = x.ui64[0];
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
#define NODEISNOTEMPTY(w) ((w) != 0)
#define ISINNODE(w, i) ((w) & (ONE << (i)))							//!< Returns a node set to a non-zero value if the bit of index \c i in node \c w is a 1-bit; otherwise a node set to \c 0.
#endif

#define NELEM(i) ((i) << LOGB)										//!< Smallest element in the entry \c i of the bitmap.
#define NODEIDX(i) ((i) >> LOGB)									//!< Node index in the bitmap of element \c i.
#define IDXINNODE(i) ((i) & MASKB)									//!< Element index in the corresponding node of the bitmap.
#define RETAINLEAST(w, i) ((w) & (NODEMAX >> (BSIZE-(i)))) 			//!< Retains \c i least significant elements of node \c w.

/*!
 * \struct BitMap
 * \brief Bitmap implementation of sets of integers of the type \f$\{ 0, 1, \ldots, n-1 \}\f$, for a specified integer \f$n \geq 0\f$.
 *
 * A bitmap is an array of bits to represent sets in which a bit of index \c i represents element \c i of the set. For a bitmap \c B,
 * \c B[\c i] \c = \c 1 means that element \c i is in the set; otherwise, \c i is not.
 *
 * Bitmaps are defined as arrays of five different types depending on the number of bits per node, namely: \c unsigned \c char (8 bits
 * per node), \c unsigned \c long \c int (32 bits per node), and \c unsigned \c long \c long \c int (64 bits per node). The type
 * currently in use is selected by means of the definition of a macro name, respectively with \c -D_ARCH8_, \c -D_ARCH32_,
 * \c -D_ARCH64_, \c -D_ARCH128_, or \c -D_ARCH256_ in the \c gcc command line.
 */
typedef struct {
	const int n;
	const int size;
	const int step;  		// in words
	const int nRnodes;
	NODETYPE * minRNode; 	//<! a lower bound for the node index of the smallest element in the set
	NODETYPE * maxRNode; 	//<! an upper bound for the node index of the greatest element in the set
	const char * ptr;		//!< Unaligned pointer to this map
	NODETYPE * R;			//!< Map
} BitMap;

/*!
 * \struct Node
 * \brief Identification of a member of a bitmap.
 */
typedef struct {
	int ind;			//!< Node index in a bitmap.
	int nelem;			//!< Smallest element that can be in this node of a bitmap.
	int cur;			//!< Current element as an index in this node.
	NODETYPE * beg1;	//!< Used for backward enumeration
	NODETYPE * end1;	//!< Used for forward enumeration
	NODETYPE * buf1;	//!< Used to enumerate
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

static inline BitMap *newBitMapVector(size_t size, size_t n) {
	int tamR = NODEIDX(n-1) + 1;
	int step = inline_ceillog2(tamR);
	int tamRS = 1 << step;
	int tam = (tamRS<<LOGW);

	BitMap * ret = (BitMap *) malloc(WSIZE+WSIZE+sizeof(BitMap)+tam*size);
	if (ret==NULL)
		return(NULL);

	BitMap aux = { n, size, step, tamR, NULL, NULL, (char *) ret, NULL };
	ret =  (BitMap *) (((char *) ret) + (WSIZE - (((intptr_t) ret) & (WSIZE-1))));
	memcpy(ret, &aux, sizeof(BitMap));

	char * maps_ptr = ((char *) ret) + sizeof(BitMap);
	ret->R = (NODETYPE *) (maps_ptr + (WSIZE - (((intptr_t) maps_ptr) & (WSIZE-1))));

	int i, r;
	for (i = 0, r = 0; i < size; i++, r += tamRS)
		memset(ret->R + r, 0, tamR<<LOGW);
	ret->minRNode = ret->R;
	ret->maxRNode = ret->R+ret->nRnodes;

	return ret;
}

/*!
 * \fn BitMap * const newBitMap(size_t n)
 * \brief Create an empty new bitmap with a specified maximum size.
 *
 * The elements that can be included in the returned bitmap are 0, ..., the specified maximum size minus 1.
 *
 * \param n Maximum number of elements that can be included in the returned bitmap.
 *
 * \return An empty bitmap.
 */
static inline BitMap * const newBitMap(size_t n) {
	return newBitMapVector(1, n);
}

static inline BitMap * const similarBitMap(BitMap const * b) {
	return newBitMap(b->n);
}

/*!
 * \fn void copyAll(BitMap * bmvec, BitMap const * abmvec)
 * \brief Copy nodes between specified bitmaps.
 *
 * \param bmvec Nodes are copied to this bitmap.
 * \param abmvec Nodes are copied from this bitmap.
 */
static inline void copyAll(BitMap * bmvec, BitMap const * abmvec) {
	memcpy(bmvec->minRNode, abmvec->minRNode, (abmvec->maxRNode-abmvec->minRNode) << LOGW);
}

/*!
 * \fn BitMap * const cloneBitMap(BitMap * const b)
 * \brief Create a copy of a specified bitmap.
 *
 * \param b Bitmap to copied to the returned bitmap.
 *
 * \return A bitmap which is identical to the specified one (in the sense that the result of any function applied to it).
 */
static inline BitMap * const cloneBitMap(BitMap const * b) {
	BitMap * ret = similarBitMap(b);
	copyAll(ret, b);
	return ret;
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

static inline void freeBitMap(BitMap *ptr) {
	free((char *) ptr->ptr);
}

static inline void subMap(BitMap * const bm, BitMap * r, int i) {
	int is = (i<<bm->step);
	r->R = bm->R + is;
	r->minRNode = r->R;
	r->maxRNode = r->R+r->nRnodes;
}

/*!
 * \fn int getElement(Node const * node)
 * \brief Return the element which is the target of a specified node
 *
 * \param node The node to be inspected
 *
 * \return The target of the specified node
 */
static inline int getElement(Node const * node) {
	return node->nelem + node->cur;
}

//static inline void setElement(Node * node, int i) {
//	node->ind = NODEIDX(i);
//	node->nelem = NELEM(node->ind);
//	node->cur = IDXINNODE(i);
//}

/*!
 * \fn void delNode(BitMap * bm, Node const * i)
 * \brief Delete a specified element's node from a specified bitmap
 *
 * If the specified element's node is not in the set, nothing happens.
 *
 * \param bm A bitmap
 * \param i An element's node
 */
static inline void delNode(BitMap * bm, Node const * i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->R+i->ind) &= ~(ONE << (i->cur));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] &= ~__oneshifted[i->cur];
#endif
}

/*!
 * \fn void delElement(BitMap * bm, int i)
 * \brief Delete a specified element from a specified bitmap
 *
 * If the specified element is not in the set, nothing happens.
 *
 * \param bm A bitmap
 * \param i An element
 */
static inline void delElement(BitMap * bm, int i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->R+NODEIDX(i)) &= ~(ONE << (IDXINNODE(i)));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	(bm->R+NODEIDX(i))->ui64[(unsigned int) __index[IDXINNODE(i)]] &= ~__oneshifted[IDXINNODE(i)];
#endif
}

/*!
 * \fn void addNode(BitMap * bm, Node const * i)
 * \brief Add a specified element's node to a specified bitmap set
 *
 * If the specified element is in the set before the call, nothing happens.
 *
 * \param bm A bitmap
 * \param i An element's node
 */
static inline void addNode(BitMap * bm, Node const * i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->R+i->ind) |= (ONE << (i->cur));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] |= __oneshifted[i->cur];
#endif
}

/*!
 * \fn void addElement(BitMap * bm, int i)
 * \brief Add a specified element to a specified bitmap set
 *
 * If the specified element is in the set before the call, nothing happens.
 *
 * \param bm A bitmap
 * \param i An element
 */
static inline void addElement(BitMap * bm, int i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->R+NODEIDX(i)) |= (ONE << (IDXINNODE(i)));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	(bm->R+NODEIDX(i))->ui64[(unsigned int) __index[IDXINNODE(i)]] |= __oneshifted[IDXINNODE(i)];
#endif
}

/*!
 * \fn void invElement(BitMap * bm, int i)
 * \brief Invert the relation of a specified element with a specified bitmap set
 *
 * If the specified element is in the set before the call, then it is deleted. Otherwise, it is included.
 *
 * \param bm A bitmap
 * \param i An element
 */
static inline void invElement(BitMap * bm, int i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->R+NODEIDX(i)) ^= (ONE << (IDXINNODE(i)));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	(bm->R+NODEIDX(i))->ui64[(unsigned int) __index[IDXINNODE(i)]] ^= __oneshifted[IDXINNODE(i)];
#endif
}

/*!
 * \fn void invNode(BitMap * bm, Node const * i)
 * \brief Invert the relation of a specified element's node with a specified bitmap set
 *
 * If the specified element's node is in the set before the call, then it is deleted. Otherwise, it is included.
 *
 * \param bm A bitmap
 * \param i An element's node
 */
static inline void invNode(BitMap * bm, Node const * i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*(bm->R+i->ind) ^= (ONE << (i->cur));
#elif defined(_ARCH128_) || defined(_ARCH256_)
	(bm->R+i->ind)->ui64[(unsigned int) __index[i->cur]] ^= __oneshifted[i->cur];
#endif
}

/*!
 * \fn int hasElement(BitMap const * bm, int i)
 * \brief Check if a specified element is in a specified bitmap set
 *
 * \param bm A bitmap
 * \param i An element
 *
 * \return a non-zero value if the element \c i is in the bitmap; otherwise return \c 0.
 */
static inline int hasElement(BitMap const * bm, int i) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	return ISINNODE(*(bm->R+NODEIDX(i)), IDXINNODE(i)) != 0;
#elif defined(_ARCH128_) || defined(_ARCH256_)
	int idx = IDXINNODE(i);
	return ISINNODE((bm->R+NODEIDX(i))->ui64[(unsigned int) __index[idx]], idx) != 0;
#endif
}

static inline BitMap * const pack(int * const v, size_t sz, size_t n) {
	BitMap * ret = newBitMap(n);
	Node node;
	int i;
	for (i = 0; i < sz; i++)
		addElement(ret, v[i]);
	return ret;
}

static inline int isEmpty(BitMap const * mvec) {
	NODETYPE * node = mvec->minRNode;
	while (node < mvec->maxRNode)
	{
		if (NODEISNOTEMPTY(*node))
			return 0;
		node++;
	}
	return 1;
}

static inline int isNotEmpty(BitMap const * mvec) {
	NODETYPE * node = mvec->minRNode;
	while (node < mvec->maxRNode)
	{
		if (NODEISNOTEMPTY(*node))
			return 1;
		node++;
	}
	return 0;
}

/*!
 * \fn int areDisjoint(BitMap const * amvec, BitMap const * bmvec)
 * \brief Check whether twp specified bitmap sets are disjoint
 *
 * \return 0 if the sets are not disjoint, and a nonzero value otherwise
 */
static inline int areDisjoint(BitMap const * amvec, BitMap const * bmvec) {
	NODETYPE * anode = amvec->minRNode;
	NODETYPE * bnode = bmvec->R;
	NODETYPE buf;

	while (anode < amvec->maxRNode) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		buf = *anode & *bnode;
#elif defined(_ARCH128_)
		_mm_store_si128 (&buf.mi, _mm_and_si128 (bnode->mi, anode->mi));
#elif defined(_ARCH256_)
		_mm256_store_pd(&buf.d64, _mm256_and_pd(bnode->md, anode->md));
#endif
		if (NODEISNOTEMPTY(buf))
			return 0;
		anode++;
		bnode++;
	}
	return 1;
}

static inline int areEqual(BitMap const * abmvec, BitMap const * bbmvec) {
	NODETYPE * abnode;
	NODETYPE * bbnode;
	NODETYPE buf;

	abnode = abmvec->minRNode;
	bbnode = bbmvec->R;

	while (abnode < abmvec->maxRNode) {
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		buf = *abnode ^ *bbnode;
#elif defined(_ARCH128_)
		_mm_store_si128 (&buf.mi, _mm_xor_si128 (bbnode->mi, abnode->mi));
#elif defined(_ARCH256_)
		_mm256_store_pd(&buf.d64, _mm256_xor_pd(bbnode->md, abnode->md));
#endif
		if (NODEISNOTEMPTY(buf))
			return 0;
		abnode++;
		bbnode++;
	}

	return 1;
}

// forward enumeration

static inline void beginWhile(BitMap const * bm, Node * node) {
	node->ind = 0;
	node->nelem = 0;
	node->cur = -1;
	node->end1 = bm->maxRNode;
	node->buf1 = bm->R;
}

static inline void nextCommon(BitMap const * bm, Node * node) {
	NODETYPE * buf2 = bm->R+node->ind;
    NODETYPE buf;
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
    buf = *node->buf1 & *buf2;
    node->cur = NODENEXTLEAST(buf, node->cur);
    if (node->cur < 0 && ++node->buf1 < node->end1)
    {
        buf = *node->buf1 & *++buf2;
		node->ind++;
		node->nelem += BSIZE;
    	while (buf == 0 && ++node->buf1 < node->end1)
    	{
            buf = *node->buf1 & *++buf2;
    		node->ind++;
    		node->nelem += BSIZE;
    	}
    	node->cur = NODELEAST(buf);
    }
#elif defined(_ARCH128_)
    _mm_store_si128 (&buf.mi, _mm_and_si128 (buf2->mi, node->buf1->mi));
    node->cur = NODENEXTLEAST(buf, node->cur);
    if (node->cur < 0 && ++node->buf1 < node->end1)
    {
    	_mm_store_si128 (&buf.mi, _mm_and_si128 ((++buf2)->mi, node->buf1->mi));
		node->ind++;
		node->nelem += BSIZE;
    	while (_mm_comieq_ss(buf.m, __zero.m) && ++node->buf1 < node->end1)
    	{
    		_mm_store_si128 (&buf.mi, _mm_and_si128 ((++buf2)->mi, node->buf1->mi));
    		node->ind++;
    		node->nelem += BSIZE;
    	}
    	node->cur = NODELEAST(buf);
    }
#elif defined(_ARCH256_)
    _mm256_store_pd(&buf.d64,_mm256_and_pd(buf2->md, node->buf1->md));
    node->cur = NODENEXTLEAST(buf, node->cur);
    if (node->cur < 0 && ++node->buf1 < node->end1)
    {
    	_mm256_store_pd(&buf.d64, _mm256_and_pd((++buf2)->md, node->buf1->md));
		node->ind++;
		node->nelem += BSIZE;
    	while (_mm256_testz_si256(buf.mi, __zero.mi) && ++node->buf1 < node->end1)
    	{
    		_mm256_store_pd(&buf.d64, _mm256_and_pd((++buf2)->md, node->buf1->md));
    		node->ind++;
    		node->nelem += BSIZE;
    	}
    	node->cur = NODELEAST(buf);
    }
#endif
}

static inline void beginCommon(BitMap const * abm, BitMap const * bm, Node * node) {
	beginWhile(bm, node);
	nextCommon(abm, node);
}

/*!
 * \fn void next(Node * node)
 * \brief Move forward a specified node in an enumeration
 *
 * If called with a node at the end of an enumeration (i.e. #end(node) returns true), than this node
 * remains at the end of an enumeration.
 *
 * \param node A node to be moved
 */
static inline void next(Node * node) {
	node->cur = NODENEXTLEAST(*node->buf1, node->cur);
	while (node->cur < 0 && ++node->buf1 < node->end1) {
		node->ind++;
		node->nelem += BSIZE;
		node->cur = NODELEAST(*node->buf1);
	}
}

/*!
 * \fn void begin(BitMap const * bm, Node * node)
 * \brief Configure a specified node to be the starting point of an enumeration of a specified bitmap.
 *
 * \param bm A bitmap
 * \param node A node to be configured
 */
static inline void begin(BitMap const * bm, Node * node) {
	beginWhile(bm, node);
	next(node);
}

/*!
 * \fn int end(Node * node)
 * \brief Test whether a specified node is at the end of an enumeration
 *
 * \param node A node to be tested
 *
 * \return 0, if the node is not at the end of an enumeration, and a nonzero value otherwise
 */
static inline int end(Node * node) {
	return node->buf1 >= node->end1;
}

static inline void copySets(Node * node, Node * anode) {
	int d = (anode->end1-anode->buf1);
	node->buf1 = node->end1-d;
	node->ind = anode->ind;
	node->nelem = anode->nelem;
	node->cur = anode->cur;
	memcpy(node->buf1, anode->buf1, d << LOGW);
}

// backward enumeration

static inline void rearWhile(BitMap const * bm, Node * node) {
	node->ind = bm->nRnodes-1;
	node->nelem = NELEM(node->ind);
	node->cur = BSIZE;
	node->beg1 = bm->minRNode-1;
	node->buf1 = bm->maxRNode-1;
}

static inline void prevCommon(BitMap const * bm, Node * node) {
	NODETYPE * buf2 = bm->R+node->ind;
    NODETYPE buf;
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
    buf = *node->buf1 & *buf2;
    node->cur = NODEPREVMOST(buf, node->cur);
    if (node->cur < 0 && --node->buf1 > node->beg1)
    {
        buf = *node->buf1 & *--buf2;
		node->ind--;
		node->nelem -= BSIZE;
    	while (buf == 0 && --node->buf1 > node->beg1)
    	{
            buf = *node->buf1 & *--buf2;
    		node->ind--;
    		node->nelem -= BSIZE;
    	}
    	node->cur = NODEMOST(buf);
    }
#elif defined (_ARCH128_)
    _mm_store_si128 (&buf.mi, _mm_and_si128 (buf2->mi, node->buf1->mi));
    node->cur = NODEPREVMOST(buf, node->cur);
    if (node->cur < 0 && --node->buf1 > node->beg1)
    {
    	_mm_store_si128 (&buf.mi, _mm_and_si128 ((--buf2)->mi, node->buf1->mi));
		node->ind--;
		node->nelem -= BSIZE;
    	while (_mm_comieq_ss(buf.m, __zero.m) && --node->buf1 > node->beg1)
    	{
    		_mm_store_si128 (&buf.mi, _mm_and_si128 ((--buf2)->mi, node->buf1->mi));
    		node->ind--;
    		node->nelem -= BSIZE;
    	}
    	node->cur = NODEMOST(buf);
    }
#elif defined (_ARCH256_)
 	_mm256_store_pd(&buf.d64, _mm256_and_pd(buf2->md, node->buf1->md));
    node->cur = NODEPREVMOST(buf, node->cur);
    if (node->cur < 0 && --node->buf1 > node->beg1)
    {
    	_mm256_store_pd(&buf.d64, _mm256_and_pd((--buf2)->md, node->buf1->md));
		node->ind--;
		node->nelem -= BSIZE;
    	while (_mm256_testz_si256(buf.mi, __zero.mi) && --node->buf1 > node->beg1)
    	{
    		_mm256_store_pd(&buf.d64,_mm256_and_pd((--buf2)->md, node->buf1->md));
    		node->ind--;
    		node->nelem -= BSIZE;
    	}
    	node->cur = NODEMOST(buf);
    }
#endif
}

static inline void rearCommon(BitMap const * abm, BitMap const * bm, Node * node) {
	rearWhile(bm, node);
	prevCommon(bm, node);
}

static inline void prev(Node * node) {
	node->cur = NODEPREVMOST(*node->buf1, node->cur);
	while (node->cur < 0 && --node->buf1 > node->beg1) {
		node->ind--;
		node->nelem -= BSIZE;
		node->cur = NODEMOST(*node->buf1);
	}
}

static inline void rear(BitMap const * bm, Node * node) {
	rearWhile(bm, node);
	prev(node);
}

static inline int head(Node * node) {
	return node->buf1 <= node->beg1;
}

//static inline void getNode(Node * node, int i) {
//	node->ind = NODEIDX(i);
//	node->nelem = NELEM(node->ind);
//	node->cur = IDXINNODE(i);
//}

static inline void printNode(Node * node) {
	printf("ind=%d  nelem=%d  cur=%d  elem=%d\n",node->ind,	node->nelem, node->cur, node->nelem+ node->cur);
}

/*!
 * \fn int cardOf(BitMap const * bmvec)
 * \brief Determines the number of elements in the specified nodes of a bitmap.
 *
 * \param bmvec Bitmap.
 *
 * \return The number of elements in the nodes of the bitmap \c bmvec.
 */
static inline int cardOf(BitMap const * bmvec) {
	int ret = 0;
	NODETYPE * node;
	for (node = bmvec->minRNode; node < bmvec->maxRNode; node++)
		ret += NODECOUNTER(*node);
	return ret;
}

static inline int * const unpack(BitMap * const b) {
	int * ret = calloc(cardOf(b), sizeof(int));
	Node node;
	int i;
	for (begin(b, &node), i=0; !end(&node); next(&node), i++) {
		ret[i]= getElement(&node);
	}
	return ret;
}

/*!
 * \fn void addAll(BitMap * bmvec, BitMap * abmvec)
 * \brief Add elements between specified bitmaps.
 *
 * This function performs a logic \c OR operation of the specified nodes.
 *
 * \param bmvec Elements are added to this bitmap set
 * \param abmvec Elements of this bitmap set are added
 */
static inline void addAll(BitMap * bmvec, BitMap const * abmvec) {
	NODETYPE * bnode;
	NODETYPE * abnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = bmvec->R, abnode = abmvec->minRNode; abnode < abmvec->maxRNode; bnode++, abnode++)
		*bnode |= *abnode;
#elif defined(_ARCH128_)
	for (bnode = bmvec->R, abnode = abmvec->minRNode; abnode < abmvec->maxRNode; bnode++, abnode++)
		_mm_store_si128 (&bnode->mi, _mm_or_si128 (bnode->mi, abnode->mi));
#elif defined(_ARCH256_)
	for (bnode = bmvec->R, abnode = abmvec->minRNode; abnode < abmvec->maxRNode; bnode++, abnode++)
		_mm256_store_pd(&bnode->d64, _mm256_or_pd(bnode->md, abnode->md));
#endif
}

static inline void moveAll(BitMap * bmvec, BitMap const * abmvec) {
	NODETYPE * bnode;
	NODETYPE * abnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = bmvec->R, abnode = abmvec->minRNode; abnode < abmvec->maxRNode; bnode++, abnode++) {
		*bnode |= *abnode;
		*abnode = 0;
	}
#elif defined(_ARCH128_)
	for (bnode = bmvec->R, abnode = abmvec->minRNode; abnode < abmvec->maxRNode; bnode++, abnode++) {
		_mm_store_si128 (&bnode->mi, _mm_or_si128 (bnode->mi, abnode->mi));
		_mm_store_si128 (&abnode->mi, __zero.mi);
	}
#elif defined(_ARCH256_)
	for (bnode = bmvec->R, abnode = abmvec->minRNode; abnode < abmvec->maxRNode; bnode++, abnode++) {
		_mm256_store_pd (&bnode->d64, _mm256_or_pd (bnode->md, abnode->md));
		_mm256_store_pd (&abnode->d64, __zero.md);
	}
#endif
}

static inline void addAllElements(BitMap * bm) {
	memset(bm->R, 0xFF, (bm->nRnodes << LOGW));
	bm->maxRNode--;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	*bm->maxRNode = RETAINLEAST(*bm->maxRNode,IDXINNODE(bm->n-1)+1);
#elif defined(_ARCH128_) || defined(_ARCH256_)
	int idx = IDXINNODE(bm->n-1);
	bm->maxRNode->ui64[(unsigned int) __index[idx]] = RETAINLEAST(bm->maxRNode->ui64[(unsigned int) __index[idx]], idx-((unsigned int) __index[idx] << LOGB)+1);
#endif

	bm->maxRNode++;
}

//!< Deletes all elements from bitmap \c R.
static inline void delAllElements(BitMap * bm) {
	memset(bm->R, 0, (bm->nRnodes) << LOGW);
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
	NODETYPE * bnode;
	NODETYPE * rbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		*bnode &= ~*rbnode;
#elif defined(_ARCH128_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm_store_si128 (&bnode->mi, _mm_andnot_si128 (rbnode->mi, bnode->mi));
#elif defined(_ARCH256_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm256_store_pd(&bnode->d64,_mm256_andnot_pd(rbnode->md, bnode->md));
#endif
}

static inline void removeFrom(Node * node, BitMap const * rbmvec) {
	NODETYPE * bnode;
	NODETYPE * rbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < node->end1; bnode++, rbnode++)
		*bnode &= ~*rbnode;
#elif defined(_ARCH128_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < node->end1; bnode++, rbnode++)
		_mm_store_si128 (&bnode->mi, _mm_andnot_si128 (rbnode->mi, bnode->mi));
#elif defined(_ARCH256_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < node->end1; bnode++, rbnode++)
		_mm256_store_pd(&bnode->d64, _mm256_andnot_pd(rbnode->md, bnode->md));
#endif
}

/*!
 * \fn void retainAll(BitMap * bmvec, BitMap * abmvec)
 * \brief Intersection of bitmaps with the result in one of them.
 *
 * This function performs a logic \c AND operation of the specified bitmap sets.
 *
 * \param bmvec Elements of this bitmap are removed.
 * \param abmvec Elements are removed from this bitmap.
 */
static inline void retainAll(BitMap * bmvec, BitMap const * rbmvec) {
	NODETYPE * bnode;
	NODETYPE * rbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		*bnode &= *rbnode;
#elif defined(_ARCH128_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm_store_si128 (&bnode->mi, _mm_and_si128 (rbnode->mi, bnode->mi));
#elif defined(_ARCH256_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm256_store_pd(&bnode->d64, _mm256_and_pd(rbnode->md, bnode->md));
#endif
}

static inline void retainFrom(BitMap * bmvec, BitMap const * rbmvec, Node * node) {
	NODETYPE * bnode;
	NODETYPE * rbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < bmvec->maxRNode; bnode++, rbnode++)
		*bnode &= *rbnode;
#elif defined(_ARCH128_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm_store_si128 (&bnode->mi, _mm_and_si128 (rbnode->mi, bnode->mi));
#elif defined(_ARCH256_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm256_store_pd(&bnode->d64, _mm256_and_pd(rbnode->md, bnode->md));
#endif
}

/*!
 * \fn void symDiff(BitMap * bmvec, BitMap const * rbmvec)
 * \brief Symmetric difference of bitmaps.
 *
 * This function performs a logic \c AND \c NEG operation of the specified bitmap sets. The elements of the second set are removed from the first.
 *
 * \param bmvec Operand bitmap
 * \param rbmvec Operand bitmap
 */
static inline void symDiff(BitMap * bmvec, BitMap const * rbmvec) {
	NODETYPE * bnode;
	NODETYPE * rbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		*bnode ^= *rbnode;
#elif defined(_ARCH128_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm_store_si128 (&bnode->mi, _mm_xor_si128 (rbnode->mi, bnode->mi));
#elif defined(_ARCH256_)
	for (bnode = bmvec->minRNode, rbnode = rbmvec->R; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm256_store_pd(&bnode->d64, _mm256_xor_pd(rbnode->md, bnode->md));
#endif
}

/*!
 * \fn void symDiffOf(BitMap * bmvec, BitMap const * abmvec, BitMap const * bbmvec)
 * \brief Symmetric difference of bitmaps with the result in a third bitmap.
 *
 * This function performs a logic \c AND \c NEG operation of the specified bitmap sets.
 *
 * \param bmvec Result bitmap
 * \param abmvec Operand bitmap
 * \param bbmvec Operand bitmap
 */
static inline void symDiffOf(BitMap * bmvec, BitMap const * abmvec, BitMap const * bbmvec) {
	NODETYPE * bnode;
	NODETYPE * abnode;
	NODETYPE * bbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = bmvec->minRNode, abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; bnode < bmvec->maxRNode; bnode++, abnode++, bbnode++)
		*bnode = *abnode ^ *bbnode;
#elif defined(_ARCH128_)
	for (bnode = bmvec->minRNode, abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; bnode < bmvec->maxRNode; bnode++, abnode++, bbnode++)
		_mm_store_si128 (&bnode->mi, _mm_xor_si128 (abnode->mi, bbnode->mi));
#elif defined(_ARCH256_)
 	for (bnode = bmvec->minRNode, abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; bnode < bmvec->maxRNode; bnode++, abnode++, bbnode++)
		_mm256_store_pd(&bnode->d64, _mm256_xor_pd(abnode->md, bbnode->md));
#endif
}

static inline void symDiffFrom(BitMap * bmvec, BitMap const * rbmvec, Node * node) {
	NODETYPE * bnode;
	NODETYPE * rbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < bmvec->maxRNode; bnode++, rbnode++)
		*bnode ^= *rbnode;
#elif defined(_ARCH128_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm_store_si128 (&bnode->mi, _mm_xor_si128 (rbnode->mi, bnode->mi));
#elif defined(_ARCH256_)
	for (bnode = node->buf1, rbnode = rbmvec->R+node->ind; bnode < bmvec->maxRNode; bnode++, rbnode++)
		_mm256_store_pd(&bnode->d64, _mm256_xor_pd(rbnode->md, bnode->md));
#endif
}

/*!
 * \fn void intersectOf(BitMap * bmvec, BitMap const * abmvec, BitMap const * bbmvec)
 * \brief Intersection of bitmaps with the result in a third bitmap.
 *
 * This function performs a logic \c AND operation of the specified bitmap sets.
 *
 * \param bmvec Result bitmap
 * \param abmvec Operand bitmap
 * \param bbmvec Operand bitmap
 */
static inline void intersectOf(BitMap * bmvec, BitMap const * abmvec, BitMap const * bbmvec) {
	NODETYPE * bnode;
	NODETYPE * abnode;
	NODETYPE * bbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (bnode = bmvec->minRNode, abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; bnode < bmvec->maxRNode; bnode++, abnode++, bbnode++)
		*bnode = *abnode & *bbnode;
#elif defined(_ARCH128_)
	for (bnode = bmvec->minRNode, abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; bnode < bmvec->maxRNode; bnode++, abnode++, bbnode++)
		_mm_store_si128 (&bnode->mi, _mm_and_si128 (abnode->mi, bbnode->mi));
#elif defined(_ARCH256_)
 	for (bnode = bmvec->minRNode, abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; bnode < bmvec->maxRNode; bnode++, abnode++, bbnode++)
		_mm256_store_pd(&bnode->d64, _mm256_and_pd(abnode->md, bbnode->md));
#endif
}

/*! \fn int isSubset(BitMap const * abmvec, BitMap const * bbmvec)
 *  \brief Determine whether a bitmap set is a subset of another bitmap set.
 *
 *  Two specified sets, represented as bitmaps, are compared in order to check whether one is a subset of the other.
 *
 * \param abmvec set to check if subset
 * \param bbmvec set to check if superset
 */
static inline int isSubset(BitMap const * abmvec, BitMap const * bbmvec) {
	NODETYPE * abnode;
	NODETYPE * bbnode;

#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
	for (abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; abnode < abmvec->maxRNode && *abnode == (*abnode & *bbnode); abnode++, bbnode++);
	return abnode == abmvec->maxRNode;
#elif defined(_ARCH128_)
	for (abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; abnode < abmvec->maxRNode && _mm_comieq_ss(abnode->m, (__m128) _mm_and_si128(abnode->mi, bbnode->mi)); abnode++, bbnode++);
	return abnode == abmvec->maxRNode;
#elif defined(_ARCH256_)
	for (abnode = abmvec->minRNode, bbnode = bbmvec->minRNode; abnode < abmvec->maxRNode && _mm256_testz_si256(abnode->mi, (__m256i) _mm256_and_pd(abnode->md, bbnode->md)); abnode++, bbnode++);
	return abnode == abmvec->maxRNode;
#endif
}

/*!
 * \fn void * map(BitMap const * s, void * par, void *base, size_t size, void (*f)(void *, const unsigned long long, void *))
 * \brief Transform specified collection pointed by \c base by applying \c f to all elements indexed by
 * the elements in the specified bitmap.
 *
 * This function applies \c f elementwise, selected through a specified bitmap set.
 *
 * \param s bitmap that selects the index of \c base
 * \param par data passed to every call to \c f
 * \param base vector containing the elements
 * \param size in bytes of each element in the array \c base
 * \param f function to apply to the selected elements
 */
static inline void * map(BitMap const * s, void * par, void *base, size_t size,
        void (*f)(void *, const unsigned long long, void *)) {
	int i;
	Node node;
	for (begin(s, &node); !end(&node); next(&node)) {
		i = getElement(&node);
		f(par,i,((char *) base)+(i*size));
	}

	return base;
}

static inline void * reduce(BitMap const * s, void * param, void *base,
        void (*f)(void *, const unsigned long long, void *dest)) {
	int i;
	Node node;
	for (begin(s, &node); !end(&node); next(&node))
		f(param,getElement(&node),base);

	return base;
}

static inline void printSetElements(BitMap const * s) {
	Node node;
	printf("[");
	for (begin(s, &node); !end(&node); next(&node))
		printf(" %d", getElement(&node));
	printf(" ]\n");
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

	printf("[");
	for (node = s->R+NODEIDX(s->n-1); node >= s->R; node--)
#if defined(_ARCH8_) || defined(_ARCH32_) || defined(_ARCH64_)
		printf(FORMX, *node);
#elif defined(_ARCH128_)
		printf(FORMX, node->ui64[1], node->ui64[0]);
#elif defined(_ARCH256_)
		printf(FORMX, node->ui64[3], node->ui64[2], node->ui64[1], node->ui64[0]);
#endif
	printf(" ]\n");
}

#endif /* BITMAP_H_ */
