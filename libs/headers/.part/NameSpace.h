#pragma once
#include "../NameSpace.h"

DLL_HIDE

struct _Crispr_NameSpace {
	mtx_t access;
	Crispr_NsRef*** refs;
	char name[];
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_NameSpace
#endif

DLL_RESTORE
