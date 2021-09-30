#include "headers/.part/NameSpace.h"
#include "headers/Error.h"
#include "headers/Memory.h"
#include "headers/String.h"

DLL_HIDE

Crispr_NameSpace* Crispr_nsCreate(const char* restrict name, const Crispr_NsRef* restrict ref,
		bool single, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	if (!name) {
		if (err)
			*err = CRISPR_ERR_NULL;
		return Crispr_nullRef(Crispr_NameSpace);
	}
	Crispr_Off len = Crispr_symLen(name, err);
	if (len < 0)
		return false;
	Crispr_NameSpace* result = Crispr_malloc(sizeof(Crispr_NameSpace) + len);
	if (single) {
		result->usercount = 0;
	} else {
		int status = mtx_init(&result->modify, mtx_plain);
		if (status == thrd_error) {
			if (err)
				*err = CRISPR_ERR_SYS;
			return Crispr_nullRef(Crispr_NameSpace);
		}
		if (!Crispr_sema_init(&result->userlock, 1, err)) {
			return Crispr_nullRef(Crispr_NameSpace);
		}
	}
	if (ref) {
		Crispr_NameSpace* parent = ref->ns;
		bool once = parent->single;
		if (!once) {
			mtx_lock(&parent->modify);
		}
	}
	return result;
}

DLL_RESTORE
