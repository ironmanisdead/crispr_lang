#include "headers/Memory.h"
#include "headers/Sema.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <threads.h>
#include <stdatomic.h>
#include <stdint.h>

DLL_HIDE

#ifndef CRISPR_EMBLOCK_INITIAL_ENTRIES
 #define CRISPR_EMBLOCK_INITIAL_ENTRIES 4
#endif
#define CRISPR_EMBLOCK_INITIAL_SIZE (CRISPR_EMBLOCK_INITIAL_ENTRIES * \
		sizeof(Crispr_MemTab))

static mtx_t crispr_memmutex;

static void memunload() {
	mtx_destroy(&crispr_memmutex);
}

DLL_LOCAL void crispr_meminit() {
	static bool done = false;
	if (done)
		return;
	done = true;
	int stat = mtx_init(&crispr_memmutex, mtx_plain);
	(void)stat;
	assert(stat == thrd_success);
	stat = atexit(&memunload);
	(void)stat;
	assert(stat == 0);
}

DLL_PUBLIC void* Crispr_malloc(Crispr_Size size) {
	crispr_meminit();
	int stat = mtx_lock(&crispr_memmutex);
	(void)stat;
	assert(stat == thrd_success);
	void* mem = malloc(size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_PUBLIC void Crispr_free(void* restrict ptr) {
	int stat = mtx_lock(&crispr_memmutex);
	(void)stat;
	assert(stat == thrd_success);
	free(ptr);
	mtx_unlock(&crispr_memmutex);
}

DLL_PUBLIC void* Crispr_realloc(void* restrict ptr, Crispr_Size size) {
	int stat = mtx_lock(&crispr_memmutex);
	(void)stat;
	assert(stat == thrd_success);
	void* mem = realloc(ptr, size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_PUBLIC void* Crispr_memcpy(void* restrict dest, const void* restrict src, Crispr_Size size) {
	int stat = mtx_lock(&crispr_memmutex);
	(void)stat;
	assert(stat == thrd_success);
	void* mem = memcpy(dest, src, size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_PUBLIC void* Crispr_memmove(void* dest, const void* src, Crispr_Size size) {
	int stat = mtx_lock(&crispr_memmutex);
	(void)stat;
	assert(stat == thrd_success);
	void* mem = memmove(dest, src, size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_PUBLIC void* Crispr_nextVarPtr(void* ptr, Crispr_Size align) {
	uintptr_t ch = (uintptr_t)ptr;
	uintptr_t calc = (ch / align) * align;
	if (calc < ch)
		calc += align;
	return (void*)calc;
}

DLL_RESTORE
