#pragma once
#include "Compare.h"
#include "Clock.h"
DLL_HIDE

typedef struct {
	const Crispr_t_Clock* clock;
	Crispr_t_s64 rawval;
	Crispr_t_s64 computed;
	Crispr_t_u64 scale;
} Crispr_t_Timer;

DLL_PUBLIC bool Crispr_f_timeNow(Crispr_t_Timer* restrict dest, const Crispr_t_Clock* restrict type,
		Crispr_t_s64 offset, Crispr_t_u64 scale, Crispr_tn_Errno* restrict stat);
DLL_PUBLIC bool Crispr_f_timeConv(Crispr_t_Timer* restrict dest, const Crispr_t_Timer* restrict src,
		const Crispr_t_Clock* restrict type, Crispr_t_u64 scale, Crispr_tn_Errno* restrict stat);
DLL_PUBLIC Crispr_t_Compare Crispr_f_timeCmp(const Crispr_t_Timer* restrict p1,
		const Crispr_t_Timer* restrict p2, Crispr_tn_Errno* restrict err);
DLL_PUBLIC Crispr_t_Compare Crispr_f_nowCmp(const Crispr_t_Timer* restrict p1);

DLL_PUBLIC extern const Crispr_t_Timer Crispr_c_present;

DLL_RESTORE
