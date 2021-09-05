#pragma once
#include "predefs.h"

DLL_HIDE

typedef struct {
	Crispr_t_s64 initial;
	Crispr_t_s64 offset;
	Crispr_t_s64 frequency;
	bool duality;
} Crispr_t_Rule;

typedef struct {
	Crispr_t_size len;
	const Crispr_t_Rule* rules;
} Crispr_t_Pattern;

DLL_PUBLIC bool Crispr_f_PatternApply(Crispr_t_s64* restrict dest, Crispr_t_s64 src,
		const Crispr_t_Pattern* restrict ref, Crispr_tn_Errno* restrict err);

DLL_PUBLIC bool Crispr_f_PatternRemove(Crispr_t_s64* restrict dest, Crispr_t_s64 src,
		const Crispr_t_Pattern* restrict ref, Crispr_tn_Errno* restrict err);

DLL_RESTORE
