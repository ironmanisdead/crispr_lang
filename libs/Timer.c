#include "headers/Timer.h"
#include "headers/Error.h"
#include <time.h>
#ifdef DLL_OS_windows
 #include <realtimeapiset.h>
#endif
DLL_HIDE

DLL_PUBLIC bool Crispr_timeNow(Crispr_Timer* restrict dest, const Crispr_Clock* clock,
		Crispr_S64 offset, Crispr_U64 scale, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	if (clock->now == CRISPR_NULL) {
		dest->clock = clock;
		dest->rawval = offset * scale;
		dest->computed = offset;
		dest->scale = scale;
		return true;
	}
	dest->scale = 0;
	if (scale == 0) {
		if (err)
			*err = CRISPR_ERRINVAL;
		return false;
	}
	if (!clock->now(&dest->rawval, err))
		return false;
	dest->rawval += offset;
	if (!Crispr_patternApply(&dest->computed, dest->rawval, &clock->pattern, err))
		return false;
	dest->scale = scale;
	dest->clock = clock;
	dest->computed /= scale;
	return true;
}

DLL_PUBLIC bool Crispr_timeConv(Crispr_Timer* restrict dest, const Crispr_Timer* restrict src,
		const Crispr_Clock* restrict clock, Crispr_U64 scale, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	dest->scale = 0;
	if ((src->scale == 0) || (scale == 0)) {
		if (err)
			*err = CRISPR_ERRDOMAIN;
		return false;
	}
	if (src->clock == clock) {
		dest->clock = clock;
		dest->scale = scale;
		dest->rawval = src->rawval;
		dest->computed = dest->rawval / scale;
		return true;
	}
	if (clock->type != src->clock->type) {
		if (err)
			*err = CRISPR_ERRCONV;
		return false;
	}
	if (src->clock->now == CRISPR_NULL) {
		if (!clock->now(&dest->rawval, err))
			return false;
		dest->rawval += src->rawval;
		if (!Crispr_patternApply(&dest->computed, dest->rawval, &clock->pattern, err))
			return false;
	} else if (clock->now == CRISPR_NULL) {
		Crispr_S64 abstime;
		if (!src->clock->now(&abstime, err))
			return false;
		dest->rawval = src->rawval - abstime;
		dest->computed = dest->rawval;
	} else if (src->clock->now == clock->now) {
		dest->rawval = src->rawval;
		if (!Crispr_patternApply(&dest->computed, dest->rawval, &clock->pattern, err))
			return false;
	} else { //if both clock->now's are different
		Crispr_S64 d1;
		if (!clock->now(&d1, err))
			return false;
		Crispr_S64 d2;
		if (!src->clock->now(&d2, err))
			return false;
		Crispr_S64 diff = d1 - d2;
		dest->rawval = src->rawval + diff;
		if (!Crispr_patternApply(&dest->computed, dest->rawval, &clock->pattern, err))
			return false;
	}
	dest->clock = clock;
	dest->scale = scale;
	dest->computed /= scale;
	return true;
}

DLL_PUBLIC Crispr_Compare Crispr_timeCmp(const Crispr_Timer* restrict p1,
		const Crispr_Timer* restrict p2, Crispr_Errno* restrict err) {
	Crispr_Timer t1;
	Crispr_Timer t2;
	if (!Crispr_timeConv(&t1, p1, p1->clock, CRISPR_TIME_NANOSECOND, err))
		return CRISPR_CMP_NQ;
	if (!Crispr_timeConv(&t2, p2, p1->clock, CRISPR_TIME_NANOSECOND, err))
		return CRISPR_CMP_NQ;
	if (t1.rawval == t2.rawval)
		return CRISPR_CMP_EQ;
	if (t1.rawval > t2.rawval)
		return CRISPR_CMP_GT;
	return CRISPR_CMP_LT;
}

DLL_PUBLIC const Crispr_Timer Crispr_c_present = { CRISPR_CLK_RELA, 0, 0, CRISPR_TIME_NANOSECOND };

DLL_RESTORE
