#pragma once
#include "predefs.h"
DLL_HIDE

struct _Crispr_t_Class {
	Crispr_t_Class* bases;
	const char* name;
};

DLL_PUBLIC Crispr_t_Class* Crispr_f_newClass(const char* restrict name, Crispr_tn_Errno* restrict);

DLL_RESTORE
