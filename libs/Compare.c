#include "headers/Compare.h"

DLL_HIDE

DLL_PUBLIC const char* Crispr_compareDesc(Crispr_Compare comp) {
	switch (comp) {
		case Crispr_cn_CMP_LT:
			return "less than";
		case Crispr_cn_CMP_GT:
			return "greater than";
		case Crispr_cn_CMP_EQ:
			return "equal to";
		case Crispr_cn_CMP_NQ:
			return "incomparable with";
		default:
			return "inconclusively comparable to";
	}
}

DLL_PUBLIC Crispr_Compare Crispr_compareNum(double d1, double d2) {
	if (d1 < d2)
		return Crispr_cn_CMP_LT;
	if (d1 > d2)
		return Crispr_cn_CMP_GT;
	if (d1 == d2)
		return Crispr_cn_CMP_EQ;
	return Crispr_cn_CMP_NQ;
}

DLL_RESTORE
