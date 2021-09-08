#pragma once
#include "../NameSpace.h"

DLL_HIDE

struct _Crispr_NameSpace {
	mtx_t modify;
	Crispr_Sema users;
	Crispr_NsRef*** refs;
	char name[];
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_NameSpace
#endif

DLL_RESTORE
