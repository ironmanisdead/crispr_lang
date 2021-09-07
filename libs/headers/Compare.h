#pragma once
#include "predefs.h"

DLL_HIDE

typedef char Crispr_Compare;

#define CRISPR_CMP_NQ (char)0 //incomparable
#define CRISPR_CMP_LT (char)1 //less-than
#define CRISPR_CMP_GT (char)2 //greater-than
#define CRISPR_CMP_EQ (char)3 //equal-to

DLL_PUBLIC const char* Crispr_compareDesc(Crispr_Compare);

DLL_PUBLIC Crispr_Compare Crispr_compareNum(double d1, double d2);

DLL_RESTORE
