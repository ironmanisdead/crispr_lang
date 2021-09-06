#include "headers/Pattern.h"
#include "headers/Errno.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_f_PatternApply(Crispr_t_s64* restrict dest, Crispr_t_s64 src,
		const Crispr_t_Pattern* restrict ref, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	*dest = src;
	if (ref->len == 0)
		return true;
	if (ref->rules == CRISPR_NULL) {
		if (err)
			*err = Crispr_cn_ENULL;
		return false;
	}
	for (Crispr_t_size i = 0; i < ref->len; i++) {
		Crispr_t_Rule rule = ref->rules[i];
//if (frequency == 0) it is implied that the rule only be applied once
		if (rule.frequency == 0) {
			if (*dest > rule.initial)
				*dest += rule.offset;
			continue;
		}
		if (!rule.duality) {
			if (rule.frequency > 0) {
				if (rule.initial > *dest)
					continue;
			} else if (rule.initial < *dest) {
				continue;
			}
		}
		if ((!rule.duality) && (rule.frequency < 0))
			rule.frequency = -rule.frequency;
		*dest += ((*dest - rule.initial) / rule.frequency) * rule.offset;
	}
	return true;
}

DLL_PUBLIC bool Crispr_f_PatternRemove(Crispr_t_s64* restrict dest, Crispr_t_s64 src,
		const Crispr_t_Pattern* restrict ref, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	if (ref->len == 0)
		return true;
	if (ref->rules == CRISPR_NULL) {
		if (err)
			*err = Crispr_cn_ENULL;
		return false;
	}
	*dest = src;
	for (Crispr_t_size i = ref->len; i > 0; i--) {
		Crispr_t_Rule rule = ref->rules[i-1];
//if (frequency == 0) it is implied that the rule only be applied once
		if (rule.frequency == 0) {
			if (*dest > rule.initial)
				*dest += rule.offset;
			continue;
		}
		if (!rule.duality) {
			if (rule.frequency > 0) {
				if (rule.initial > *dest)
					continue;
			} else if (rule.initial < *dest) {
				continue;
			}
		}
		if ((!rule.duality) && (rule.frequency < 0))
			rule.frequency = -rule.frequency;
		*dest += ((*dest - rule.initial) / rule.frequency) * -rule.offset;
	}
	return true;
}
