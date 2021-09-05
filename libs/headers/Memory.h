#pragma once
#include "predefs.h"

DLL_HIDE

DLL_PUBLIC void* Crispr_f_malloc(Crispr_t_size);
DLL_PUBLIC void* Crispr_f_realloc(void* restrict, Crispr_t_size);
DLL_PUBLIC void Crispr_f_free(void* restrict);
DLL_PUBLIC void* Crispr_f_memcpy(void* restrict, const void* restrict, Crispr_t_size);
DLL_PUBLIC void* Crispr_f_memmove(void*, const void*, Crispr_t_size);
DLL_LOCAL void crispr_meminit();

DLL_RESTORE
