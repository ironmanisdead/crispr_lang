#include "headers/Thread.h"
#include "headers/Stack.h"
#include "headers/Errno.h"
#include "headers/Memory.h"
#include "headers/Sema.h"
#include <stdatomic.h>
DLL_HIDE

static _Atomic Crispr_t_ulong nThread;
static thread_local Crispr_t_ulong id = 1;

static void crispr_thrdinit() {
	static bool done = false;
	if (done)
		return;
	atomic_init(&nThread, 2);
	done = true;
}

typedef struct {
	Crispr_t_Sema lock;
	Crispr_t_Stack* ret;
	const Crispr_t_Stack* args;
	void (*callback)(Crispr_t_Stack* ret, const Crispr_t_Stack* args);
} Crispr_t_ThreadStart;

static int Crispr_f_thread_start(void* restrict info) {
	Crispr_t_ThreadStart* starter = (Crispr_t_ThreadStart*)info;
	Crispr_f_sema_unlock(&starter->lock, Crispr_M_NULL);
	id = atomic_fetch_add(&nThread, 1);
	return 0;
}

DLL_PUBLIC bool Crispr_f_thread_init(Crispr_t_Thread* restrict dest,
		void (*callback)(Crispr_t_Stack* ret, const Crispr_t_Stack* args),
		const Crispr_t_Stack* args, Crispr_tn_Errno* stat) {
	crispr_meminit();
	crispr_thrdinit();
	dest->valid = false;
	if (stat)
		*stat = Crispr_cn_ENOERR;
	Crispr_t_Stack* mem = Crispr_f_malloc(sizeof(Crispr_t_Stack));
	if (!mem) {
		if (stat)
			*stat = Crispr_cn_ENOMEM;
		return false;
	}
	Crispr_t_ThreadStart starter;
	Crispr_f_sema_init(&starter.lock, 1, Crispr_M_NULL);
	starter.ret = mem;
	starter.args = args;
	starter.callback = callback;
	switch (thrd_create(&dest->internal, &Crispr_f_thread_start, &starter)) {
		case thrd_success:
			Crispr_f_sema_lock(&starter.lock, false, Crispr_M_NULL, Crispr_M_NULL);
			return true;
		case thrd_nomem:
			if (stat)
				*stat = Crispr_cn_ENOMEM;
			return false;
	}
	*stat = Crispr_cn_EUNKNOWN;
	return false;
}

DLL_PUBLIC Crispr_t_ulong Crispr_f_thread_id() {
	return id;
}

DLL_RESTORE
