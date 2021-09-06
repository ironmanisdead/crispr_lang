#include "headers/Clock.h"
#include "headers/Errno.h"
#include <time.h>
#ifdef DLL_OS_windows
 #include <realtimeapi.h>
#endif
DLL_HIDE

static bool crispr_f_clock_tai(Crispr_t_s64* restrict res, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
#ifdef DLL_OS_unix
	struct timespec present;
	if (clock_gettime(CLOCK_BOOTTIME, &present) != 0) {
		if (err)
			*err = Crispr_cn_EUNKNOWN;
		return false;
	}
	*res = (present.tv_sec * CRISPR_TIME_SECOND) + present.tv_nsec;
#elif defined DLL_OS_windows
	if (!QueryUnbiasedInterruptTime(res)) {
		if (err)
			*err = Crispr_cn_EUNKNOWN;
		return false;
	}
	*res *= 100;
#endif
	return true;
}

static bool crispr_f_clock_utc(Crispr_t_s64* restrict res, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	struct timespec present;
	if (timespec_get(&present, TIME_UTC) != TIME_UTC) {
		if (err)
			*err = Crispr_cn_EUNKNOWN;
		return false;
	}
	*res = (present.tv_sec * CRISPR_TIME_SECOND) + present.tv_nsec;
	return true;
}

DLL_PUBLIC const Crispr_t_Clock _Crispr_cn_CLK_ABS = {
	"CLK_ABS: Accurate timestamp with implementation-defined epoch.",
	&crispr_f_clock_tai,
	{ 0, CRISPR_NULL, },
};

DLL_PUBLIC const Crispr_t_Clock _Crispr_cn_CLK_UTC = {
	"CLK_UTC: Clock holding time adjusted from UTC epoch",
	&crispr_f_clock_utc,
	{ 0, CRISPR_NULL, },
};

DLL_RESTORE
