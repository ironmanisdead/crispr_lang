#pragma once
#include "predefs.h"
DLL_HIDE

struct _Crispr_Class {
	Crispr_Class* bases;
	const char* name;
};

DLL_PUBLIC Crispr_Class* Crispr_newClass(const char* restrict name, Crispr_Errno* restrict);

DLL_RESTORE
