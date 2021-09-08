#include "headers/.part/NameSpace.h"
#include "headers/Errno.h"
#include "headers/Memory.h"

DLL_HIDE

Crispr_NameSpace* Crispr_nsCreate(const char* restrict name, const Crispr_NsRef* restrict ref, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	if (!name) {
		if (err)
			*err = CRISPR_ERRNULL;
		return CRISPR_NULL;
	}
	Crispr_Size len = 0;
	char ch = *name;
	if (ch == '\0') {
		if (err)
			*err = CRISPR_ERREMPTY;
		return false;
	} else if ((ch >= '0') && (ch <= '9')) {
		if (err)
			*err = CRISPR_ERRINVAL;
		return CRISPR_NULL;
	}
	while ((ch = name[len]) != '\0') {
		if (((ch < 'A') || (ch > 'Z')) &&
				((ch < 'a') || (ch > 'z')) &&
				((ch < '0') || (ch > '9')) && (ch != '_')) {
			if (err)
				*err = CRISPR_ERRINVAL;
			return CRISPR_NULL;
		}
		len++;
	}
	Crispr_NameSpace* result = Crispr_malloc(sizeof(Crispr_NameSpace) + len);
	int status = mtx_init(&result->modify, mtx_plain);
	if (status == thrd_error) {
		if (err)
			*err = CRISPR_ERRUNKNOWN;
		return CRISPR_NULL;
	}
	if (!Crispr_sema_init(&result->users, 1, err)) {
		return CRISPR_NULL;
	}
	return result;
}

DLL_RESTORE
