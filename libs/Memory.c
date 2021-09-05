#include "headers/Memory.h"
#include "headers/Sema.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <threads.h>
#include <stdatomic.h>

DLL_HIDE

#ifndef Crispr_MEMBLOCK_INITIAL_ENTRIES
 #define Crispr_MEMBLOCK_INITIAL_ENTRIES 4
#endif
#define Crispr_MEMBLOCK_INITIAL_SIZE (Crispr_MEMBLOCK_INITIAL_ENTRIES * \
		sizeof(Crispr_t_MemTab))

static mtx_t crispr_memmutex;

static void memunload() {
	mtx_destroy(&crispr_memmutex);
}

DLL_LOCAL void crispr_meminit() {
	static bool done = false;
	if (done)
		return;
	done = true;
	assert(mtx_init(&crispr_memmutex, mtx_plain) == thrd_success);
	assert(atexit(&memunload) == 0);
}

DLL_PUBLIC void* Crispr_f_malloc(Crispr_t_size size) {
	crispr_meminit();
	assert(mtx_lock(&crispr_memmutex) == thrd_success);
	void* mem = malloc(size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_PUBLIC void Crispr_f_free(void* restrict ptr) {
	assert(mtx_lock(&crispr_memmutex) == thrd_success);
	free(ptr);
	mtx_unlock(&crispr_memmutex);
}

DLL_PUBLIC void* Crispr_f_realloc(void* restrict ptr, Crispr_t_size size) {
	assert(mtx_lock(&crispr_memmutex) == thrd_success);
	void* mem = realloc(ptr, size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_PUBLIC void* Crispr_f_memcpy(void* restrict dest, const void* restrict src, Crispr_t_size size) {
	assert(mtx_lock(&crispr_memmutex) == thrd_success);
	void* mem = memcpy(dest, src, size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_PUBLIC void* Crispr_f_memmove(void* dest, const void* src, Crispr_t_size size) {
	assert(mtx_lock(&crispr_memmutex) == thrd_success);
	void* mem = memmove(dest, src, size);
	mtx_unlock(&crispr_memmutex);
	return mem;
}

DLL_RESTORE
