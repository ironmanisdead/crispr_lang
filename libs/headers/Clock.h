#pragma once
#include "predefs.h"
#include "Pattern.h"

DLL_HIDE

typedef struct {
	const char* timename;
	bool (*now)(Crispr_t_s64* restrict res, Crispr_tn_Errno* restrict err);
	Crispr_t_Pattern pattern;
} Crispr_t_Clock;

DLL_PUBLIC extern const Crispr_t_Clock _Crispr_cn_CLK_ABS;
DLL_PUBLIC extern const Crispr_t_Clock _Crispr_cn_CLK_UTC;

#define Crispr_M_CLK_RELA ((const Crispr_t_Clock*)0)
#define Crispr_M_CLK_ABS &_Crispr_cn_CLK_ABS
#define Crispr_M_CLK_UTC &_Crispr_cn_CLK_UTC

#if (defined __GNUC__) && ! (defined DLL_EXPORT_LIB_crispr_api)
 #pragma GCC poison _Crispr_cn_CLK_ABS
 #pragma GCC poison _Crispr_cn_CLK_UTC
#endif

#define Crispr_M_TIME_NANOSECOND 1LL
#define Crispr_M_TIME_MILLISECOND 1000000LL
#define Crispr_M_TIME_SECOND (1000 * Crispr_M_TIME_MILLISECOND)
#define Crispr_M_TIME_MINUTE (60 * Crispr_M_TIME_SECOND)
#define Crispr_M_TIME_HOUR (3600 * Crispr_M_TIME_SECOND)
#define Crispr_M_TIME_HEXHOUR (6 * Crispr_M_TIME_HOUR)
#define Crispr_M_TIME_SIDEREAL ((5 * Crispr_M_TIME_HOUR) + (48 * Crispr_M_TIME_MINUTE) + (56 * Crispr_M_TIME_SECOND))
#define Crispr_M_TIME_DAY (4 * Crispr_M_TIME_HEXHOUR)
#define Crispr_M_TIME_YEAR ((365 * Crispr_M_TIME_DAY) + Crispr_M_TIME_SIDEREAL)
#define Crispr_M_TIME_QUADRENNIAL (4 * Crispr_M_TIME_YEAR)
#define Crispr_M_TIME_DECADE (10 * Crispr_M_TIME_YEAR)
#define Crispr_M_TIME_SCORE (20 * Crispr_M_TIME_YEAR)
#define Crispr_M_TIME_GETTYSBURG ((4 * Crispr_M_TIME_SCORE) + (7 * Crispr_M_TIME_YEAR))
#define Crispr_M_TIME_CENTURY (25 * Crispr_M_TIME_QUADRENNIAL)

DLL_RESTORE
