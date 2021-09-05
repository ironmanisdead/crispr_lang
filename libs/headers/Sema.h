#pragma once
#include "predefs.h"
#include "Timer.h"
#include <threads.h>

DLL_HIDE

typedef struct _Crispr_t_Sema Crispr_t_Sema;

typedef struct _Crispr_t_SemSched Crispr_t_SemSched;

struct _Crispr_t_SemSched {
	Crispr_t_Sema* volatile _Atomic parent; //schedbusy if being used, and NULL if unusable
	Crispr_t_SemSched* next; //pointer to next lock
	Crispr_t_SemSched** prev; //pointer to pointer from previous semsched
	cnd_t lock; //lock to this scheduler
};

struct _Crispr_t_Sema {
	volatile _Atomic char status; //keep track of semaphore status
	Crispr_t_ulong limit; //limit of semaphore (0 if plain mutex)
	volatile _Atomic Crispr_t_uint threads; //amount of threads using this mutex
	mtx_t access; //access aquisition mutex
	union {
		struct { //semaphore specific values
			Crispr_t_ulong counter; //status of current thread
			Crispr_t_SemSched* schedptr; //pointer to the first threading object
		};
		cnd_t waiter; //mutex waiter
	};
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_t_Sema
 #pragma GCC poison _Crispr_t_SemSched
 #ifndef DLL_EXPORT_LIB_crispr_api
  #pragma GCC poison _Crispr_tn_SemStat
 #endif
#endif

//	Crispr_f_sema_init should only ever be used on an unconstructed mutex,
//as undefined behavior will result otherwise.
//	Conversly the rest of the Crispr_f_sema_* functions should never be called on an unconstructed mutex,
//as undefined behavior will result as well.
DLL_PUBLIC bool Crispr_f_sema_init(Crispr_t_Sema* restrict dest, Crispr_t_ulong limit, Crispr_tn_Errno* restrict err);
DLL_PUBLIC bool Crispr_f_sema_destroy(Crispr_t_Sema* target, Crispr_tn_Errno* restrict stat);
DLL_PUBLIC bool Crispr_f_sema_lock(Crispr_t_Sema* src, bool term, const Crispr_t_Timer* restrict wait, Crispr_tn_Errno* restrict stat);
DLL_PUBLIC bool Crispr_f_sema_unlock(Crispr_t_Sema* target, Crispr_tn_Errno* restrict stat);
DLL_PUBLIC bool Crispr_f_sema_reset(Crispr_t_Sema* target, Crispr_t_ulong limit);
DLL_PUBLIC bool Crispr_f_sema_schedinit(Crispr_t_SemSched* restrict dest, Crispr_t_Sema* src, bool term, Crispr_tn_Errno* restrict stat);
DLL_PUBLIC bool Crispr_f_sema_schedcancel(Crispr_t_SemSched* sched, Crispr_tn_Errno* restrict stat);
DLL_PUBLIC bool Crispr_f_sema_schedfinish(Crispr_t_SemSched* sched, const Crispr_t_Timer* restrict wait, Crispr_tn_Errno* restrict stat);

DLL_RESTORE
