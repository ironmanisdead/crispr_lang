#pragma once
#include "predefs.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_symLen(Crispr_Size* restrict res, const char* restrict sym, Crispr_Errno* restrict err);
DLL_PUBLIC Crispr_Size Crispr_strLen(const char* restrict str);
DLL_PUBLIC Crispr_Size Crispr_strCpy(char* restrict dst, const char* restrict src);

DLL_RESTORE
