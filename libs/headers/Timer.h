#pragma once
#include "Compare.h"
#include "Clock.h"
DLL_HIDE

typedef struct {
	const Crispr_Clock* clock;
	Crispr_S64 rawval;
	Crispr_S64 computed;
	Crispr_U64 scale;
} Crispr_Timer;

DLL_PUBLIC bool Crispr_timeNow(Crispr_Timer* restrict dest, const Crispr_Clock* restrict type,
		Crispr_S64 offset, Crispr_U64 scale, Crispr_Errno* restrict stat);
DLL_PUBLIC bool Crispr_timeConv(Crispr_Timer* restrict dest, const Crispr_Timer* restrict src,
		const Crispr_Clock* restrict type, Crispr_U64 scale, Crispr_Errno* restrict stat);
DLL_PUBLIC Crispr_Compare Crispr_timeCmp(const Crispr_Timer* restrict p1,
		const Crispr_Timer* restrict p2, Crispr_Errno* restrict err);
DLL_PUBLIC Crispr_Compare Crispr_nowCmp(const Crispr_Timer* restrict p1);

DLL_PUBLIC extern const Crispr_Timer Crispr_c_present;

DLL_RESTORE
