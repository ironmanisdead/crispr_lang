#include "headers/Timer.h"
#include "headers/Errno.h"
#include <time.h>
#ifdef DLL_OS_WINDOWS
 #include <realtimeapiset.h>
#endif
DLL_HIDE

DLL_PUBLIC bool Crispr_f_timeNow(Crispr_t_Timer* restrict dest, const Crispr_t_Clock* clock,
		Crispr_t_s64 offset, Crispr_t_u64 scale, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	if (clock == CRISPR_CLK_RELA) {
		dest->clock = CRISPR_CLK_RELA;
		dest->rawval = offset * scale;
		dest->computed = offset;
		dest->scale = scale;
		return true;
	}
	dest->scale = 0;
	if (scale == 0) {
		if (err)
			*err = Crispr_cn_EINVAL;
		return false;
	}
	if (!clock->now(&dest->rawval, err))
		return false;
	dest->rawval += offset;
	if (!Crispr_f_PatternApply(&dest->computed, dest->rawval, &clock->pattern, err))
		return false;
	dest->scale = scale;
	dest->clock = clock;
	dest->computed /= scale;
	return true;
}

DLL_PUBLIC bool Crispr_f_timeConv(Crispr_t_Timer* restrict dest, const Crispr_t_Timer* restrict src,
		const Crispr_t_Clock* restrict clock, Crispr_t_u64 scale, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	dest->scale = 0;
	if ((src->scale == 0) || (scale == 0)) {
		if (err)
			*err = Crispr_cn_EDOMAIN;
		return false;
	}
	if (src->clock == clock) {
		dest->clock = clock;
		dest->scale = scale;
		dest->rawval = src->rawval;
		dest->computed = dest->rawval / scale;
		return true;
	}
	if (src->clock == CRISPR_CLK_RELA) {
		if (!clock->now(&dest->rawval, err))
			return false;
		dest->rawval += src->rawval;
		if (!Crispr_f_PatternApply(&dest->computed, dest->rawval, &clock->pattern, err))
			return false;
	} else if (clock == CRISPR_CLK_RELA) {
		Crispr_t_s64 abstime;
		if (!src->clock->now(&abstime, err))
			return false;
		dest->rawval = src->rawval - abstime;
		dest->computed = dest->rawval;
	} else if (src->clock->now == clock->now) {
		dest->rawval = src->rawval;
		if (!Crispr_f_PatternApply(&dest->computed, dest->rawval, &clock->pattern, err))
			return false;
	} else { //if both clock->now's are different
		Crispr_t_s64 d1;
		if (!clock->now(&d1, err))
			return false;
		Crispr_t_s64 d2;
		if (!src->clock->now(&d2, err))
			return false;
		Crispr_t_s64 diff = d1 - d2;
		dest->rawval = src->rawval + diff;
		if (!Crispr_f_PatternApply(&dest->computed, dest->rawval, &clock->pattern, err))
			return false;
	}
	dest->clock = clock;
	dest->scale = scale;
	dest->computed /= scale;
	return true;
}

DLL_PUBLIC Crispr_t_Compare Crispr_f_timeCmp(const Crispr_t_Timer* restrict p1,
		const Crispr_t_Timer* restrict p2, Crispr_tn_Errno* restrict err) {
	Crispr_t_Timer t1;
	Crispr_t_Timer t2;
	if (!Crispr_f_timeConv(&t1, p1, CRISPR_CLK_ABS, CRISPR_TIME_NANOSECOND, err))
		return Crispr_cn_CMP_NQ;
	if (!Crispr_f_timeConv(&t2, p2, CRISPR_CLK_ABS, CRISPR_TIME_NANOSECOND, err))
		return Crispr_cn_CMP_NQ;
	if (t1.rawval == t2.rawval)
		return Crispr_cn_CMP_EQ;
	if (t1.rawval > t2.rawval)
		return Crispr_cn_CMP_GT;
	return Crispr_cn_CMP_LT;
}

DLL_PUBLIC const Crispr_t_Timer Crispr_c_present = { CRISPR_CLK_RELA, 0, 0, CRISPR_TIME_NANOSECOND };

DLL_RESTORE
