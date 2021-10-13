#include "headers/Pattern.h"
#include "headers/Error.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_patternApply(Crispr_S64* restrict dest, Crispr_S64 src,
		const Crispr_Pattern* restrict ref, Crispr_Errno* restrict err) {
	Crispr_refSet(err, CRISPR_ERR_NOERR, false);
	*dest = src;
	if (ref->len == 0)
		return true;
	if (ref->rules == Crispr_nullRef(Crispr_Rule)) {
		Crispr_refSet(err, CRISPR_ERR_NULL, false);
		return false;
	}
	for (Crispr_Size i = 0; i < ref->len; i++) {
		Crispr_Rule rule = ref->rules[i];
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

DLL_PUBLIC bool Crispr_patternRemove(Crispr_S64* restrict dest, Crispr_S64 src,
		const Crispr_Pattern* restrict ref, Crispr_Errno* restrict err) {
	Crispr_refSet(err, CRISPR_ERR_NOERR, false);
	if (ref->len == 0)
		return true;
	if (ref->rules == Crispr_nullRef(Crispr_Rule)) {
		Crispr_refSet(err, CRISPR_ERR_NULL, false);
		return false;
	}
	*dest = src;
	for (Crispr_Size i = ref->len; i > 0; i--) {
		Crispr_Rule rule = ref->rules[i-1];
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
