#include "headers/.part/NameSpace.h"
#include "headers/Error.h"
#include "headers/Memory.h"
#include "headers/Symbol.h"

DLL_HIDE

Crispr_NameSpace* Crispr_nsCreate(const char* restrict name, const Crispr_NsRef* restrict ref, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	if (!name) {
		if (err)
			*err = CRISPR_ERRNULL;
		return CRISPR_NULL;
	}
	Crispr_Size len;
	if (!Crispr_symLen(&len, name, err))
		return false;
	Crispr_NameSpace* result = Crispr_malloc(sizeof(Crispr_NameSpace) + len);
	int status = mtx_init(&result->modify, mtx_plain);
	if (status == thrd_error) {
		if (err)
			*err = CRISPR_ERRSYS;
		return CRISPR_NULL;
	}
	if (!Crispr_sema_init(&result->users, 1, err)) {
		return CRISPR_NULL;
	}
	return result;
}

DLL_RESTORE
