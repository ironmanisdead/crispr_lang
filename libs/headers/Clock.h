#pragma once
#include "predefs.h"
#include "Pattern.h"

DLL_HIDE

typedef struct {
	const char* timename;
	bool (*now)(Crispr_S64* restrict res, Crispr_Errno* restrict err);
	Crispr_Pattern pattern;
} Crispr_Clock;

DLL_PUBLIC extern const Crispr_Clock _Crispr_cn_CLK_ABS;
DLL_PUBLIC extern const Crispr_Clock _Crispr_cn_CLK_UTC;

#define CRISPR_CLK_RELA ((const Crispr_Clock*)0)
#define CRISPR_CLK_ABS &_Crispr_cn_CLK_ABS
#define CRISPR_CLK_UTC &_Crispr_cn_CLK_UTC

#if (defined __GNUC__) && ! (defined DLL_EXPORT_LIB_crispr_api)
 #pragma GCC poison _Crispr_cn_CLK_ABS
 #pragma GCC poison _Crispr_cn_CLK_UTC
#endif

#define CRISPR_TIME_NANOSECOND 1LL
#define CRISPR_TIME_MILLISECOND 1000000LL
#define CRISPR_TIME_SECOND (1000 * CRISPR_TIME_MILLISECOND)
#define CRISPR_TIME_MINUTE (60 * CRISPR_TIME_SECOND)
#define CRISPR_TIME_HOUR (3600 * CRISPR_TIME_SECOND)
#define CRISPR_TIME_HEXHOUR (6 * CRISPR_TIME_HOUR)
#define CRISPR_TIME_SIDEREAL ((5 * CRISPR_TIME_HOUR) + (48 * CRISPR_TIME_MINUTE) + (56 * CRISPR_TIME_SECOND))
#define CRISPR_TIME_DAY (4 * CRISPR_TIME_HEXHOUR)
#define CRISPR_TIME_YEAR ((365 * CRISPR_TIME_DAY) + CRISPR_TIME_SIDEREAL)
#define CRISPR_TIME_QUADRENNIAL (4 * CRISPR_TIME_YEAR)
#define CRISPR_TIME_DECADE (10 * CRISPR_TIME_YEAR)
#define CRISPR_TIME_SCORE (20 * CRISPR_TIME_YEAR)
#define CRISPR_TIME_GETTYSBURG ((4 * CRISPR_TIME_SCORE) + (7 * CRISPR_TIME_YEAR))
#define CRISPR_TIME_CENTURY (25 * CRISPR_TIME_QUADRENNIAL)

DLL_RESTORE
