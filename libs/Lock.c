#include "headers/Lock.h"
#include "headers/.part/Lock.h"
#include "headers/Memory.h"
#include "headers/Error.h"
#include <threads.h>
#include <stdatomic.h>
#include <assert.h>

DLL_HIDE

DLL_LOCAL char _Crispr_flagGet(volatile Crispr_Lock* obj, bool wait) {
	char val = CRISPR_LOCK_USED;
	if (wait)
		val |= CRISPR_LOCK_SPEC;
	while (true) {
		char stat = atomic_exchange(&obj->flags, val);
		if (!(stat & CRISPR_LOCK_USED))
			return stat;
		if (stat & CRISPR_LOCK_SPEC)
			thrd_yield();
	}
}

DLL_LOCAL void _Crispr_flagSet(volatile Crispr_Lock* obj, char setting) {
	assert(atomic_load(&obj->flags) & CRISPR_LOCK_USED);
	atomic_store(&obj->flags, setting);
}

DLL_PUBLIC void Crispr_lock_init(Crispr_Lock* restrict obj) {
	atomic_init(&obj->flags, CRISPR_LOCK_NONE);
}

DLL_PUBLIC bool Crispr_lock_alloc(volatile Crispr_Lock* obj, Crispr_Size size, bool overwrite, Crispr_Errno* restrict err) {
	Crispr_refSet(err, CRISPR_ERR_NOERR, false);
	char lock = _Crispr_flagGet(obj, false);
	if ((!overwrite) && (lock != CRISPR_LOCK_NONE)) {
		Crispr_refSet(err, CRISPR_ERR_ACCESS, false);
		return false;
	}
	void* edit = Crispr_malloc(size);
	if (!edit) {
		_Crispr_flagSet(obj, lock);
		Crispr_refSet(err, CRISPR_ERR_NOMEM, false);
		return false;
	}
	if (lock & CRISPR_LOCK_RPTR) {
		Crispr_free(obj->entry.edit);
	}
	obj->entry.edit = edit;
	_Crispr_flagSet(obj, CRISPR_LOCK_RPTR);
	return true;
}

DLL_RESTORE
