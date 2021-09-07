#include "headers/Clock.h"
#include "headers/Errno.h"
#include <time.h>
#ifdef DLL_OS_windows
 #include <realtimeapi.h>
#endif
DLL_HIDE

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

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_ABS = {
	"CLK_ABS: Accurate timestamp with implementation-defined epoch.",
	&crispr_clock_tai,
	{ 0, CRISPR_NULL, },
};

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_UTC = {
	"CLK_UTC: Clock holding time adjusted from UTC epoch",
	&crispr_clock_utc,
	{ 0, CRISPR_NULL, },
};

DLL_RESTORE
