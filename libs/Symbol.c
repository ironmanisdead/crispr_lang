#include "headers/Symbol.h"
#include "headers/Errno.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_symLen(Crispr_Size* restrict res, const char* restrict sym, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	if (!sym) {
		if (err)
			*err = CRISPR_ERRNULL;
		return false;
	}
	char ch = sym[0];
	if (ch == '\0') {
		*res = 0;
		if (err)
			*err = CRISPR_ERREMPTY;
		return false;
	}
	if (((ch < 'A') || (ch > 'Z')) && ((ch < 'a') || (ch > 'z')) && (ch != '_')) {
		if (err)
			*err = CRISPR_ERRSYMBOL;
		return false;
	}
	Crispr_Size idx;
	for (idx = 1; (ch = sym[idx]) != '\0'; idx++) {
		if (((ch < 'A') || (ch > 'Z')) && ((ch < 'a') || (ch > 'z')) && (ch != '_')) {
			if (err)
				*err = CRISPR_ERRSYMBOL;
			return false;
		}
	}
	*res = idx;
	return true;
}

DLL_RESTORE
