#include "headers/Clock.h"
#include "headers/Error.h"
#include "headers/.part/Clock.h"
#include <time.h>
#ifdef DLL_OS_WINDOWS
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
		*err = CRISPR_ERR_NOERR;
#ifdef DLL_OS_UNIX
	struct timespec present;
	if (clock_gettime(CLOCK_BOOTTIME, &present) != 0) {
		if (err)
			*err = CRISPR_ERR_SYS;
		return false;
	}
	*res = (present.tv_sec * CRISPR_TIME_SECOND) + present.tv_nsec;
#elif defined DLL_OS_WINDOWS
	if (!QueryUnbiasedInterruptTime(res)) {
		if (err)
			*err = CRISPR_ERR_SYS;
		return false;
	}
	*res *= 100;
#endif
	return true;
}

static bool crispr_clock_utc(Crispr_S64* restrict res, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	struct timespec present;
	if (timespec_get(&present, TIME_UTC) != TIME_UTC) {
		if (err)
			*err = CRISPR_ERR_SYS;
		return false;
	}
	*res = (present.tv_sec * CRISPR_TIME_SECOND) + present.tv_nsec;
	return true;
}

static bool crispr_clock_cpu(Crispr_S64* restrict res, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	clock_t val = clock();
	if (val == -1) {
		if (err)
			*err = CRISPR_ERR_NOERR;
		return false;
	}
	*res = (((Crispr_S64)val) * CRISPR_TIME_SECOND) / CLOCKS_PER_SEC;
	return true;
}

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_RELA = {
	"CLK_RELA: Timestamp measuring temporal difference..",
	Crispr_nullFun(bool, Crispr_S64* restrict, Crispr_Errno* restrict),
	{ 0, Crispr_nullRef(Crispr_Rule), },
	CRISPR_TMTYP_REAL,
};

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_ABS = {
	"CLK_ABS: Accurate timestamp with implementation-defined epoch.",
	&crispr_clock_tai,
	{ 0, Crispr_nullRef(Crispr_Rule), },
	CRISPR_TMTYP_REAL,
};

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_UTC = {
	"CLK_UTC: Clock holding time adjusted from UTC epoch",
	&crispr_clock_utc,
	{ 0, Crispr_nullRef(Crispr_Rule), },
	CRISPR_TMTYP_REAL,
};

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_CPU0 = {
	"CLK_CPU0 measures difference between cpu times.",
	Crispr_nullFun(bool, Crispr_S64* restrict, Crispr_Errno* restrict),
	{ 0, Crispr_nullRef(Crispr_Rule), },
	CRISPR_TMTYP_CPU,
};

DLL_PUBLIC const Crispr_Clock _Crispr_cn_CLK_CPU1 = {
	"CLK_CPU0 measures difference between cpu times.",
	&crispr_clock_cpu,
	{ 0, Crispr_nullRef(Crispr_Rule), },
	CRISPR_TMTYP_CPU,
};

DLL_RESTORE
