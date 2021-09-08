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
	return true;
}

DLL_RESTORE
