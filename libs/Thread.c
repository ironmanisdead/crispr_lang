#include "headers/Thread.h"
#include "headers/Stack.h"
#include "headers/Error.h"
#include "headers/Memory.h"
#include "headers/Sema.h"
#include <stdatomic.h>
DLL_HIDE

static _Atomic Crispr_Ulong nThread;
static thread_local Crispr_Ulong id = 1;

static void crispr_thrdinit() {
	static bool done = false;
	if (done)
		return;
	atomic_init(&nThread, 2);
	done = true;
}

typedef struct {
	Crispr_Sema lock;
	Crispr_Stack* ret;
	const Crispr_Stack* args;
	void (*callback)(Crispr_Stack* ret, const Crispr_Stack* args);
} Crispr_ThreadStart;

static int Crispr_thread_start(void* restrict info) {
	Crispr_ThreadStart* starter = (Crispr_ThreadStart*)info;
	Crispr_sema_unlock(&starter->lock, Crispr_nullRef(Crispr_Errno));
	id = atomic_fetch_add(&nThread, 1);
	return 0;
}

DLL_PUBLIC bool Crispr_thread_init(Crispr_Thread* restrict dest,
		void (*callback)(Crispr_Stack* ret, const Crispr_Stack* args),
		const Crispr_Stack* args, Crispr_Errno* stat) {
	crispr_meminit();
	crispr_thrdinit();
	dest->valid = false;
	if (stat)
		*stat = CRISPR_ERRNOERR;
	Crispr_Stack* mem = Crispr_malloc(sizeof(Crispr_Stack));
	if (!mem) {
		if (stat)
			*stat = CRISPR_ERRNOMEM;
		return false;
	}
	Crispr_ThreadStart starter;
	Crispr_sema_init(&starter.lock, 1, Crispr_nullRef(Crispr_Errno));
	starter.ret = mem;
	starter.args = args;
	starter.callback = callback;
	switch (thrd_create(&dest->internal, &Crispr_thread_start, &starter)) {
		case thrd_success:
			Crispr_sema_lock(&starter.lock, false, Crispr_nullRef(Crispr_Timer), Crispr_nullRef(Crispr_Errno));
			return true;
		case thrd_nomem:
			if (stat)
				*stat = CRISPR_ERRNOMEM;
			return false;
	}
	*stat = CRISPR_ERRSYS;
	return false;
}

DLL_PUBLIC Crispr_Ulong Crispr_thread_id() {
	return id;
}

DLL_RESTORE
