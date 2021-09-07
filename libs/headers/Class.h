#pragma once
#include "predefs.h"
DLL_HIDE

struct _Crispr_Class {
	bool finality;
	const Crispr_NameSpace* ns;
	Crispr_Size namelen;
	Crispr_Size baselen;
	struct {
		Crispr_Size start;
		Crispr_Size inner;
	} Alignment;
	char[] extra;
};

#ifdef __GNUC__
 #pragma poison _Crispr_Class
#endif

DLL_PUBLIC const Crispr_Class* Crispr_newClass(const char* restrict name, Crispr_Errno* restrict);

DLL_RESTORE
