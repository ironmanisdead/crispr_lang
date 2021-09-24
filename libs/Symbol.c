#include "headers/Symbol.h"
#include "headers/Error.h"

DLL_HIDE

DLL_PUBLIC Crispr_Size Crispr_strLen(const char* restrict str) {
	Crispr_Size i = 0;
	while (str[i] != '\0')
		++i;
	return i;
}

DLL_PUBLIC Crispr_Size Crispr_strCpy(char* restrict dst, const char* restrict src) {
	Crispr_Size i = 0;
	while (src[i] != '\0') {
		dst[i] = src[i];
		++i;
	}
	dst[i] = '\0';
	return i;
}

DLL_PUBLIC bool Crispr_symLen(Crispr_Size* restrict res, const char* restrict sym, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	if (!sym) {
		if (err)
			*err = CRISPR_ERR_NULL;
		return false;
	}
	char ch = sym[0];
	if (ch == '\0') {
		*res = 0;
		if (err)
			*err = CRISPR_ERR_EMPTY;
		return false;
	}
	if (((ch < 'A') || (ch > 'Z')) && ((ch < 'a') || (ch > 'z')) && (ch != '_')) {
		if (err)
			*err = CRISPR_ERR_SYMBOL;
		return false;
	}
	Crispr_Size idx;
	for (idx = 1; (ch = sym[idx]) != '\0'; idx++) {
		if (((ch < 'A') || (ch > 'Z')) && ((ch < 'a') || (ch > 'z')) &&
				((ch < '0') || (ch > '9')) && (ch != '_')) {
			if (err)
				*err = CRISPR_ERR_SYMBOL;
			return false;
		}
	}
	*res = idx;
	return true;
}

DLL_RESTORE
