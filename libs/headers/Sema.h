#pragma once
#include "predefs.h"
#include "Timer.h"
#include <threads.h>

DLL_HIDE

typedef struct _Crispr_Sema Crispr_Sema;

typedef struct _Crispr_SemSched Crispr_SemSched;

struct _Crispr_SemSched {
	Crispr_Sema* volatile _Atomic parent; //schedbusy if being used, and NULL if unusable
	Crispr_SemSched* next; //pointer to next lock
	Crispr_SemSched** prev; //pointer to pointer from previous semsched
	cnd_t lock; //lock to this scheduler
};

struct _Crispr_Sema {
	volatile _Atomic char status; //keep track of semaphore status
	Crispr_Ulong limit; //limit of semaphore (0 if plain mutex)
	volatile _Atomic Crispr_Uint threads; //amount of threads using this mutex
	mtx_t access; //access aquisition mutex
	union {
		struct { //semaphore specific values
			Crispr_Ulong counter; //status of current thread
			Crispr_SemSched* schedptr; //pointer to the first threading object
		};
		cnd_t waiter; //mutex waiter
	};
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_Sema
 #pragma GCC poison _Crispr_SemSched
#endif

//	Crispr_sema_init should only ever be used on an unconstructed mutex,
//as undefined behavior will result otherwise.
//	Conversly the rest of the Crispr_sema_* functions should never be called on an unconstructed mutex,
//as undefined behavior will result as well.
DLL_PUBLIC bool Crispr_sema_init(Crispr_Sema* restrict dest, Crispr_Ulong limit, Crispr_Errno* restrict err);
DLL_PUBLIC bool Crispr_sema_destroy(Crispr_Sema* target, Crispr_Errno* restrict stat);
DLL_PUBLIC bool Crispr_sema_lock(Crispr_Sema* src, bool term, const Crispr_Timer* restrict wait, Crispr_Errno* restrict stat);
DLL_PUBLIC bool Crispr_sema_unlock(Crispr_Sema* target, Crispr_Errno* restrict stat);
DLL_PUBLIC bool Crispr_sema_reset(Crispr_Sema* target, Crispr_Ulong limit);
DLL_PUBLIC bool Crispr_sema_schedInit(Crispr_SemSched* restrict dest, Crispr_Sema* src, bool term, Crispr_Errno* restrict stat);
DLL_PUBLIC bool Crispr_sema_schedCancel(Crispr_SemSched* sched, Crispr_Errno* restrict stat);
DLL_PUBLIC bool Crispr_sema_schedFinish(Crispr_SemSched* sched, const Crispr_Timer* restrict wait, Crispr_Errno* restrict stat);

DLL_RESTORE
