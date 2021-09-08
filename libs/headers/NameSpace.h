#pragma once
#include "Sema.h"

DLL_HIDE

typedef struct _Crispr_NsRef {
	const Crispr_NameSpace* ns;
	Crispr_SemSched user;
} Crispr_NsRef;

#ifdef __GNUC__
 #pragma GCC poison _Crispr_NsRef
#endif

DLL_PUBLIC Crispr_NameSpace* Crispr_nsCreate(const char* restrict name,
		const Crispr_NsRef* parent, Crispr_Errno* restrict);

DLL_PUBLIC bool Crispr_nsRef(Crispr_NsRef* restrict dest, const Crispr_NameSpace* ns);

DLL_PUBLIC bool Crispr_nsGet(Crispr_NsRef* restrict dest, const Crispr_NsRef* src,
		const char* restrict name, Crispr_Errno* restrict);

DLL_RESTORE
