#pragma once
#include "../NameSpace.h"

DLL_HIDE

struct _Crispr_NameSpace {
	bool single; //is the namespace in a single threaded environment
	mtx_t modify; //modify mutex (only initialized in poly-threaded environment)
	union {
		Crispr_Sema userlock; //if multi-threaded
		Crispr_Size usercount; //if single-threaded
	};
	Crispr_NsRef parent; //reference to parent namespace
	Crispr_NsRef sibling; //reference to sibling namespace
	Crispr_NsRef child; //reference to child namespace
	char name[]; //name of namespace
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_NameSpace
#endif

DLL_RESTORE
