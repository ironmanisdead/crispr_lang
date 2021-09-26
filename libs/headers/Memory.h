#pragma once
#include "predefs.h"

DLL_HIDE

DLL_PUBLIC void* Crispr_malloc(Crispr_Size); //thread-safe malloc
DLL_PUBLIC void* Crispr_realloc(void* restrict, Crispr_Size); //thread-safe realloc
DLL_PUBLIC void Crispr_free(void* restrict); //thread-safe free
DLL_PUBLIC void* Crispr_memcpy(void* restrict, const void* restrict, Crispr_Size); //thread-safe memcpy
DLL_PUBLIC void* Crispr_memmove(void*, const void*, Crispr_Size); //thread-safe memmove
DLL_LOCAL void crispr_meminit(); //initializes memory for thread-safe usage (internal)
DLL_PUBLIC void* Crispr_nextVarPtr(void* src, Crispr_Size align); //gets the next pointer for an alignment

#define Crispr_nextPtr(type, ptr) ((type*)Crispr_nextVarPtr((void*)ptr, _Alignof(type))) //gets next aligned pointer for type

DLL_RESTORE
