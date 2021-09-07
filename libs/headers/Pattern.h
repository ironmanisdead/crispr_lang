#pragma once
#include "predefs.h"

DLL_HIDE

typedef struct {
	Crispr_S64 initial;
	Crispr_S64 offset;
	Crispr_S64 frequency;
	bool duality;
} Crispr_Rule;

typedef struct {
	Crispr_Size len;
	const Crispr_Rule* rules;
} Crispr_Pattern;

DLL_PUBLIC bool Crispr_patternApply(Crispr_S64* restrict dest, Crispr_S64 src,
		const Crispr_Pattern* restrict ref, Crispr_Errno* restrict err);

DLL_PUBLIC bool Crispr_patternRemove(Crispr_S64* restrict dest, Crispr_S64 src,
		const Crispr_Pattern* restrict ref, Crispr_Errno* restrict err);

DLL_RESTORE
