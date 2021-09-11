#include "headers/Clock.h"
#include "headers/Errno.h"
#include "headers/.part/Clock.h"
#include <time.h>
#ifdef DLL_OS_windows
 #include <realtimeapi.h>
#endif
DLL_HIDE

const Crispr_TimeType _Crispr_cn_TMTYP_REAL = {
	"Real time",
	"Measures time at a consistent and deterministic rate relative to a fixed moment in wall-clock time.",
};

const Crispr_TimeType _Crispr_cn_TMTYP_CPU = {
	"CPU time",
	"Measures time concerning the individual process CPU usage.",
};

static bool crispr_clock_tai(Crispr_S64* restrict res, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
#ifdef DLL_OS_unix
	struct timespec present;
	if (clock_gettime(CLOCK_BOOTTIME, &present) != 0) {
		if (err)
			*err = CRISPR_ERRUNKNOWN;
		return false;
	}
	*res = (present.tv_sec * CRISPR_TIME_SECOND) + present.tv_nsec;
#elif defined DLL_OS_windows
	if (!QueryUnbiasedInterruptTime(res)) {
		if (err)
			*err = CRISPR_ERRUNKNOWN;
		return false;
	}
	*res *= 100;
#endif
	return true;
}

static bool crispr_clock_utc(Crispr_S64* restrict res, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	struct timespec present;
	if (timespec_get(&present, TIME_UTC) != TIME_UTC) {
		if (err)
			*err = CRISPR_ERRUNKNOWN;
		return false;
	}
	*res = (present.tv_sec * CRISPR_TIME_SECOND) + present.tv_nsec;
	return true;
}

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_RELA = {
	"CLK_RELA: Timestamp measuring distance from present",
	CRISPR_NULL,
	{ 0, CRISPR_NULL, },
	CRISPR_TMTYP_REAL,
};

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_ABS = {
	"CLK_ABS: Accurate timestamp with implementation-defined epoch.",
	&crispr_clock_tai,
	{ 0, CRISPR_NULL, },
	CRISPR_TMTYP_REAL,
};

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_UTC = {
	"CLK_UTC: Clock holding time adjusted from UTC epoch",
	&crispr_clock_utc,
	{ 0, CRISPR_NULL, },
	CRISPR_TMTYP_REAL,
};

DLL_RESTORE
