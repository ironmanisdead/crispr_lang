#pragma once
#include "Sema.h"

DLL_HIDE

struct _Crispr_State {
	Crispr_Sema statelock;
	Crispr_Size lockmem;
	Crispr_Size lockent;
	Crispr_Sema* typelocks;
	Crispr_Size typemem;
	Crispr_Size typeent;
	Crispr_Class* types;
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_State
#endif

DLL_PUBLIC bool Crispr_newState(Crispr_State* restrict state, Crispr_Errno* restrict status);

DLL_PUBLIC bool Crispr_delState(Crispr_State* state, Crispr_Errno* restrict status);

DLL_RESTORE
