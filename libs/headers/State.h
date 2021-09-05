#pragma once
#include "Sema.h"

DLL_HIDE

struct _Crispr_t_State {
	Crispr_t_Sema statelock;
	Crispr_t_size lockmem;
	Crispr_t_size lockent;
	Crispr_t_Sema* typelocks;
	Crispr_t_size typemem;
	Crispr_t_size typeent;
	Crispr_t_Class* types;
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_t_State
#endif

DLL_PUBLIC bool Crispr_f_newState(Crispr_t_State* restrict state, Crispr_tn_Errno* restrict status);

DLL_PUBLIC bool Crispr_f_delState(Crispr_t_State* state, Crispr_tn_Errno* restrict status);

DLL_RESTORE
