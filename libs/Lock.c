#include "headers/Lock.h"
#include "headers/.part/Lock.h"
#include "headers/Memory.h"
#include "headers/Errno.h"
#include <threads.h>
#include <stdatomic.h>
#include <assert.h>

DLL_HIDE

DLL_LOCAL char _Crispr_f_FlagGet(volatile Crispr_t_Lock* obj, bool wait) {
	char val = Crispr_M_LOCK_USED;
	if (wait)
		val |= Crispr_M_LOCK_SPEC;
	while (true) {
		char stat = atomic_exchange(&obj->flags, val);
		if (!(stat & Crispr_M_LOCK_USED))
			return stat;
		if (stat & Crispr_M_LOCK_SPEC)
			thrd_yield();
	}
}

DLL_LOCAL void _Crispr_f_FlagSet(volatile Crispr_t_Lock* obj, char setting) {
	assert(atomic_load(&obj->flags) & Crispr_M_LOCK_USED);
	atomic_store(&obj->flags, setting);
}

DLL_PUBLIC void Crispr_f_lock_init(Crispr_t_Lock* restrict obj) {
	atomic_init(&obj->flags, Crispr_M_LOCK_NONE);
}

DLL_PUBLIC bool Crispr_f_lock_alloc(volatile Crispr_t_Lock* obj, Crispr_t_size size, bool overwrite, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	char lock = _Crispr_f_FlagGet(obj, false);
	if ((!overwrite) && (lock != Crispr_M_LOCK_NONE)) {
		if (err)
			*err = Crispr_cn_EACCESS;
		return false;
	}
	void* edit = Crispr_f_malloc(size);
	if (!edit) {
		_Crispr_f_FlagSet(obj, lock);
		if (err)
			*err = Crispr_cn_ENOMEM;
		return false;
	}
	if (lock & Crispr_M_LOCK_RPTR) {
		Crispr_f_free(obj->entry.edit);
	}
	obj->entry.edit = edit;
	_Crispr_f_FlagSet(obj, Crispr_M_LOCK_RPTR);
	return true;
}

DLL_RESTORE
