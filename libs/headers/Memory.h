#pragma once
#include "predefs.h"

DLL_HIDE

DLL_PUBLIC void* Crispr_malloc(Crispr_Size); //thread-safe malloc
DLL_PUBLIC void* Crispr_realloc(void* restrict, Crispr_Size); //thread-safe realloc
DLL_PUBLIC void Crispr_free(void* restrict); //thread-safe free
DLL_PUBLIC void* Crispr_memcpy(void* restrict, const void* restrict, Crispr_Size); //thread-safe memcpy
DLL_PUBLIC void* Crispr_memmove(void*, const void*, Crispr_Size); //thread-safe memmove
DLL_LOCAL void crispr_meminit(); //initializes memory for thread-safe usage (internal)
DLL_PUBLIC void* Crispr_nextVarPtr(const void* src, Crispr_Size align); //gets the next pointer for an alignment
//gets the next offset given base, initial offset, and alignment
DLL_PUBLIC Crispr_Size Crispr_nextVarBase(const void* base, Crispr_Size offset, Crispr_Size align);

#define Crispr_nextPtr(type, ptr) ((type*)Crispr_nextVarPtr(ptr, _Alignof(type))) //gets next aligned pointer for type
#define Crispr_nextBase(type, base, off) Crispr_nextVarBase(base, off, _Alignof(type)) //sets offset

DLL_RESTORE
