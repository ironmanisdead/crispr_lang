#pragma once
#include "predefs.h"
#include "Loop.h"

DLL_HIDE

DLL_PUBLIC Crispr_Size Crispr_strLen(const char* restrict str);
DLL_PUBLIC Crispr_Size Crispr_strCpy(char* restrict dst, const char* restrict src);

DLL_PUBLIC Crispr_Off Crispr_strIter(const char* restrict str, Crispr_LoopStr iter, Crispr_Errno* restrict err);
#define Crispr_symLen(str, err) Crispr_strIter(str, &Crispr_symName, err)

DLL_PUBLIC Crispr_LoopStat Crispr_symName(const char* restrict str, Crispr_Size off, Crispr_Errno* restrict err);
DLL_PUBLIC Crispr_LoopStat Crispr_symVm(const char* restrict str, Crispr_Size off, Crispr_Errno* restrict err);

DLL_RESTORE
