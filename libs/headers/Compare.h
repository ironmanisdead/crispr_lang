#pragma once
#include "predefs.h"

DLL_HIDE

typedef enum {
	Crispr_cn_CMP_GT, //greater-than
	Crispr_cn_CMP_LT, //less-than
	Crispr_cn_CMP_EQ, //equal-to
	Crispr_cn_CMP_NQ, //incomparable
} Crispr_t_Compare;

DLL_PUBLIC const char* Crispr_f_Compare_text(Crispr_t_Compare);

DLL_PUBLIC Crispr_t_Compare Crispr_f_Compare_num(double d1, double d2);

DLL_RESTORE
