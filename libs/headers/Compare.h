#pragma once
#include "predefs.h"

DLL_HIDE

typedef enum {
	Crispr_cn_CMP_GT, //greater-than
	Crispr_cn_CMP_LT, //less-than
	Crispr_cn_CMP_EQ, //equal-to
	Crispr_cn_CMP_NQ, //incomparable
} Crispr_Compare;

DLL_PUBLIC const char* Crispr_compareDesc(Crispr_Compare);

DLL_PUBLIC Crispr_Compare Crispr_compareNum(double d1, double d2);

DLL_RESTORE
