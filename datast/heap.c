#include <heap.h>

#include <stdio.h>
#include <string.h>

typedef unsigned char byte;

int heapup(void *base, int i, size_t size,
		int (*compar)(const void *, const void *)) {
	int is = i * size;
	byte * baseb = (byte *) base;
	char x[size];
	memcpy(x, baseb + is, size);
	int f = i + 1;
	int fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
	f = f >> 1;
	while (is > 0 && compar(x, baseb + fs) < 0) {
		memcpy(baseb + is, baseb + fs, size);
		is = fs;
		fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
		f = f >> 1;
	}
	memcpy(baseb + is, x, size);

	return is / size;
}

int heapup_a(void *base, int i, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *)) {
	int is = i * size;
	byte * baseb = (byte *) base;
	char x[size];
	memcpy(x, baseb + is, size);
	int f = i + 1;
	int fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
	f = f >> 1;
	while (is > 0 && compar(x, baseb + fs) < 0) {
		memcpy(baseb + is, baseb + fs, size);
		pos[hash(baseb + is)] = is / size;
		is = fs;
		fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
		f = f >> 1;
	}
	memcpy(baseb + is, x, size);
	pos[hash(baseb + is)] = is / size;

	return is / size;
}

int heapup_r(void *b, int i, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg) {
	int is = i * size;
	byte * base = (byte *) b;
	char x[size];
	memcpy(x, base + is, size);
	int f = i + 1;
	int fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
	f = f >> 1;
	while (is > 0 && compar(x, base + fs, arg) < 0) {
		memcpy(base + is, base + fs, size);
		is = fs;
		fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
		f = f >> 1;
	}
	memcpy(base + is, x, size);

	return is / size;
}

void heapoffer(void *x, void *b, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *)) {
	int is = nmemb * size;
	byte * base = (byte *) b;
//	memcpy(base+is, x, size);
	int f = nmemb + 1;
	int fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
	f = f >> 1;
	while (is > 0 && compar(x, base + fs) < 0) {
		memcpy(base + is, base + fs, size);
		is = fs;
		fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
		f = f >> 1;
	}
	memcpy(base + is, x, size);
}

void heapoffer_a(void *x, void *b, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *)) {
	int is = nmemb * size;
	byte * base = (byte *) b;
//	memcpy(base+is, x, size);
	int f = nmemb + 1;
	int fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
	f = f >> 1;
	while (is > 0 && compar(x, base + fs) < 0) {
		memcpy(base + is, base + fs, size);
		pos[hash(base + is)] = is / size;
		is = fs;
		fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
		f = f >> 1;
	}
	memcpy(base + is, x, size);
	pos[hash(base + is)] = is / size;
}

void heapoffer_r(void *x, void *b, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg) {
	int is = nmemb * size;
	byte * base = (byte *) b;
//	memcpy(base+is, x, size);
	int f = nmemb + 1;
	int fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
	f = f >> 1;
	while (is > 0 && compar(x, base + fs, arg) < 0) {
		memcpy(base + is, base + fs, size);
		is = fs;
		fs = f & 0x01 ? (is >> 1) - size : (is >> 1) - (size >> 1);
		f = f >> 1;
	}
	memcpy(base + is, x, size);
}

static int heapdown_s(int is, void *aux, void *b, size_t nmemb, int ls,
		size_t size, int (*compar)(const void *, const void *)) {
	byte * base = (byte *) b;
	int os, ss;
	do {
		os = (is << 1) + size;
		ss = os <= ls && (compar(base + os, base + is) < 0) ? os : is;
		os += size;
		ss = os <= ls && (compar(base + os, base + ss) < 0) ? os : ss;
		if (ss != is) {
			memcpy(aux, base + is, size);
			memcpy(base + is, base + ss, size);
			memcpy(base + ss, aux, size);
			is = ss;
		} else
			return is;
	} while (1);
	return 0;
}

static int heapdown_s_a(int is, void *aux, void *b, size_t nmemb, int ls,
		size_t size, int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *)) {
	byte * base = (byte *) b;
	int os, ss;
	do {
		os = (is << 1) + size;
		ss = os <= ls && (compar(base + os, base + is) < 0) ? os : is;
		os += size;
		ss = os <= ls && (compar(base + os, base + ss) < 0) ? os : ss;
		if (ss != is) {
			memcpy(aux, base + is, size);
			memcpy(base + is, base + ss, size);
			pos[hash(base + is)] = is / size;
			memcpy(base + ss, aux, size);
			pos[hash(base + ss)] = ss / size;
			is = ss;
		} else
			return is;
	} while (1);
	return 0;
}

static int heapdown_s_r(int is, void *aux, void *b, size_t nmemb, int ls,
		size_t size, int (*compar)(const void *, const void *, void *),
		void * arg) {
	byte * base = (byte *) b;
	int os, ss;
	do {
		os = (is << 1) + size;
		ss = os <= ls && (compar(base + os, base + is, arg) < 0) ? os : is;
		os += size;
		ss = os <= ls && (compar(base + os, base + ss, arg) < 0) ? os : ss;
		if (ss != is) {
			memcpy(aux, base + is, size);
			memcpy(base + is, base + ss, size);
			memcpy(base + ss, aux, size);
			is = ss;
		} else
			return is;
	} while (1);
	return 0;
}

int heapdown(void *b, int i, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *)) {
	if (i >= nmemb - 1)
		return i;
	char aux[size];
	int ls = (nmemb - 1) * size;
	return heapdown_s(i * size, aux, b, nmemb, ls, size, compar) / size;
}

int heapdown_a(void *b, int i, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *)) {
	if (i >= nmemb - 1)
		return i;
	char aux[size];
	int ls = (nmemb - 1) * size;
	return heapdown_s_a(i * size, aux, b, nmemb, ls, size, compar, pos, hash)
			/ size;
}

int heapdown_r(void *b, int i, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg) {
	if (i >= nmemb - 1)
		return i;
	char aux[size];
	int ls = (nmemb - 1) * size;
	return heapdown_s_r(i * size, aux, b, nmemb, ls, size, compar, arg) / size;
}

void heappoll(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *)) {
	if (nmemb < 2)
		return;
	byte * baseb = (byte *) base;
	int ls = (nmemb - 2) * size;
	memcpy(baseb, baseb + ls + size, size);
	if (nmemb > 2)
		heapdown_s(0, baseb + ls + size, baseb, nmemb - 1, ls, size, compar);
}

void heappoll_a(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *)) {
	if (nmemb < 2)
		return;
	byte * baseb = (byte *) base;
	int ls = (nmemb - 2) * size;
	memcpy(baseb, baseb + ls + size, size);
	pos[hash(baseb)] = 0;
	if (nmemb > 2)
		heapdown_s_a(0, baseb + ls + size, baseb, nmemb - 1, ls, size, compar,
				pos, hash);
}

void heappoll_r(void *b, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg) {
	if (nmemb < 2)
		return;
	byte * base = (byte *) b;
	int ls = (nmemb - 2) * size;
	memcpy(base, base + ls + size, size);
	if (nmemb > 2)
		heapdown_s_r(0, base + ls + size, base, nmemb - 1, ls, size, compar,
				arg);
}

void heapify(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *)) {
	char aux[size];
	int ls = (nmemb - 1) * size;
	int is = nmemb & 0x01 ? (ls >> 1) - size : (ls >> 1) - (size >> 1);
	for (; is >= 0; is -= size)
		heapdown_s(is, aux, (byte *) base, nmemb, ls, size, compar);
}

void heapify_a(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *)) {
	char aux[size];
	int ls = (nmemb - 1) * size;
	int is = nmemb & 0x01 ? (ls >> 1) - size : (ls >> 1) - (size >> 1);
	for (; is >= 0; is -= size)
		heapdown_s_a(is, aux, (byte *) base, nmemb, ls, size, compar, pos, hash);
}

void heapify_r(void *b, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg) {
	char aux[size];
	int ls = (nmemb - 1) * size;
	int is = nmemb & 0x01 ? (ls >> 1) - size : (ls >> 1) - (size >> 1);
	for (; is >= 0; is -= size)
		heapdown_s_r(is, aux, (byte *) b, nmemb, ls, size, compar, arg);
}
