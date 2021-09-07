#pragma once
#include "predefs.h"

DLL_HIDE

DLL_PUBLIC void* Crispr_malloc(Crispr_Size);
DLL_PUBLIC void* Crispr_realloc(void* restrict, Crispr_Size);
DLL_PUBLIC void Crispr_free(void* restrict);
DLL_PUBLIC void* Crispr_memcpy(void* restrict, const void* restrict, Crispr_Size);
DLL_PUBLIC void* Crispr_memmove(void*, const void*, Crispr_Size);
DLL_LOCAL void crispr_meminit();

DLL_RESTORE
