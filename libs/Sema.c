#include "headers/Sema.h"
#include "headers/.part/Sema.h"
#include "headers/Memory.h"
#include "headers/Errno.h"
#include <stdatomic.h>
#include <limits.h>
#include <assert.h>

DLL_HIDE

static Crispr_t_Sema crispr_c_schedbusy; //constant to tell semaphore busy status

DLL_PUBLIC bool Crispr_f_sema_init(Crispr_t_Sema* restrict target, Crispr_t_ulong limit, Crispr_tn_Errno* restrict err) {
	atomic_init(&target->status, Crispr_cn_Sema_DEAD);
	atomic_init(&target->threads, 0);
	if (mtx_init(&target->access, mtx_plain) == thrd_error) {
		if (err)
			*err = Crispr_cn_EUNKNOWN;
		return false;
	}
	if (limit > 0) {
		atomic_store(&target->status, Crispr_cn_Sema_IDLE);
		target->limit = limit;
		target->counter = 1;
		target->schedptr = Crispr_M_NULL;
		return true;
	}
	{
		int i = cnd_init(&target->waiter);
		if (i == thrd_error) {
			mtx_destroy(&target->access);
			if (err)
				*err = Crispr_cn_EUNKNOWN;
			return false;
		} else if (i == thrd_nomem) {
			mtx_destroy(&target->access);
			if (err)
				*err = Crispr_cn_ENOMEM;
			return false;
		}
	}
	target->limit = 0;
	atomic_store(&target->status, Crispr_cn_Sema_IDLE);
	return true;
}

static char crispr_f_sema_statlock(Crispr_t_Sema* target) {
	char stat;
	while (true) {
		stat = atomic_exchange(&target->status, Crispr_cn_Sema_USED);
		if (stat != Crispr_cn_Sema_USED) {
			return stat;
		}
	}
}

static void crispr_f_sema_statrel(Crispr_t_Sema* target, char stat) {
	char status = atomic_exchange(&target->status, stat);
	(void)status;
	assert(status == Crispr_cn_Sema_USED);
}

static char crispr_f_sema_statload(Crispr_t_Sema* target) {
	char stat;
	while (true) {
		stat = atomic_exchange(&target->status, Crispr_cn_Sema_USED);
		if (stat != Crispr_cn_Sema_USED)
			break;
	}
	atomic_store(&target->status, stat);
	return stat;
}

static char crispr_f_sema_statxchng(Crispr_t_Sema* target, char stat) {
	char lock = crispr_f_sema_statlock(target);
	crispr_f_sema_statrel(target, stat);
	return lock;
}

static void crispr_f_sema_shift(Crispr_t_SemSched* rm) {
	cnd_destroy(&rm->lock);
	Crispr_t_SemSched* next = rm->next;
	Crispr_t_SemSched** prev = rm->prev;
	*prev = next;
	if (next)
		next->prev = prev;
	rm->parent = Crispr_M_NULL;
}

static Crispr_t_SemSched** crispr_f_sema_latest(Crispr_t_Sema* target) {
	Crispr_t_SemSched** latest = &target->schedptr;
	Crispr_t_SemSched* current = *latest;
	while (current) {
		latest = &current->next;
		current = *latest;
	}
	return latest;
}

static char crispr_f_sema_time(struct timespec* restrict dest, const Crispr_t_Timer* restrict src) {
	if (src) {
		if ((src->clock == Crispr_M_CLK_RELA) && (src->computed == 0)) {
			return 2;
		} else {
			Crispr_t_Timer time;
			assert(Crispr_f_timeConv(&time, src, Crispr_M_CLK_UTC, Crispr_M_TIME_NANOSECOND, Crispr_M_NULL));
			dest->tv_sec = time.computed / Crispr_M_TIME_SECOND;
			dest->tv_nsec = time.computed % Crispr_M_TIME_SECOND;
			return 1;
		}
	} else {
		return 0;
	}
}

DLL_PUBLIC bool Crispr_f_sema_lock(Crispr_t_Sema* target, bool term, const Crispr_t_Timer* restrict wait, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	char lock = crispr_f_sema_statlock(target);
	if (lock & (Crispr_cn_Sema_TERM | Crispr_cn_Sema_KILL | Crispr_cn_Sema_DEAD)) {
		crispr_f_sema_statrel(target, lock);
		if (err)
			*err = Crispr_cn_ESTALE;
		return false;
	}
	{
		Crispr_t_uint count = atomic_fetch_add(&target->threads, 1);
		(void)count;
		assert(count < UINT_MAX);
	}
	//statrel should be called after the emergency thread counter is incremented
	if (term) {
		crispr_f_sema_statrel(target, Crispr_cn_Sema_TERM);
	} else {
		crispr_f_sema_statrel(target, Crispr_cn_Sema_IDLE);
	}
	struct timespec timeconv;
	char present = crispr_f_sema_time(&timeconv, wait);
	{
		int stat = mtx_lock(&target->access);
		(void)stat;
		assert(stat == thrd_success);
	}
	if (target->limit > 0) {
		if (target->counter > 0) {
			target->counter -= 1;
			mtx_unlock(&target->access);
			atomic_fetch_sub(&target->threads, 1);
			return true;
		}
		Crispr_t_SemSched** latest = crispr_f_sema_latest(target);
		Crispr_t_SemSched waiter;
		atomic_init(&waiter.parent, &crispr_c_schedbusy);
		waiter.next = Crispr_M_NULL;
		waiter.prev = latest;
		int i = cnd_init(&waiter.lock);
		if (i == thrd_error) {
			mtx_unlock(&target->access);
			atomic_fetch_sub(&target->threads, 1);
			if (err)
				*err = Crispr_cn_EUNKNOWN;
			return false;
		} else if (i == thrd_nomem) {
			mtx_unlock(&target->access);
			atomic_fetch_sub(&target->threads, 1);
			if (err)
				*err = Crispr_cn_ENOMEM;
			return false;
		}
		*latest = &waiter;
		int result;
		struct timespec beginning = { 0, 0 };
		switch (present) {
			case 0:
				result = cnd_wait(&waiter.lock, &target->access);
				break;
			case 1:
				result = cnd_timedwait(&waiter.lock, &target->access, &timeconv);
				break;
			default:
				result = cnd_timedwait(&waiter.lock, &target->access, &beginning); 
		}
		cnd_destroy(&waiter.lock);
		if (result == thrd_timedout) {
			crispr_f_sema_shift(&waiter);
			mtx_unlock(&target->access);
			atomic_fetch_sub(&target->threads, 1);
			if (err)
				*err = Crispr_cn_ETIMEDOUT;
			return false;
		} else {
			char load = crispr_f_sema_statlock(target);
			if (load == Crispr_cn_Sema_KILL) {
				crispr_f_sema_statrel(target, Crispr_cn_Sema_KILL);
				mtx_unlock(&target->access);
				atomic_fetch_sub(&target->threads, 1);
				if (err)
					*err = Crispr_cn_EDEAD;
				return false;
			} else if ((load == Crispr_cn_Sema_TERM) && (atomic_load(&target->threads) < 2)) {
				mtx_unlock(&target->access);
				mtx_destroy(&target->access);
				atomic_store(&target->threads, 0);
				crispr_f_sema_statrel(target, Crispr_cn_Sema_DEAD);
				return true;
			} else {
				crispr_f_sema_statrel(target, load);
				crispr_f_sema_shift(&waiter);
			}
		}
	} else {
		int result;
		struct timespec beginning = { 0, 0 };
		switch (present) {
			case 0:
				result = cnd_wait(&target->waiter, &target->access);
				break;
			case 1:
				result = cnd_timedwait(&target->waiter, &target->access, &timeconv);
				break;
			default:
				result = cnd_timedwait(&target->waiter, &target->access, &beginning);
		}
		char lock = crispr_f_sema_statload(target);
		if (result == thrd_timedout) {
			mtx_unlock(&target->access);
			atomic_fetch_sub(&target->threads, 1);
			if (err)
				*err = Crispr_cn_ETIMEDOUT;
			return false;
		} else if (lock == Crispr_cn_Sema_KILL) {
			mtx_unlock(&target->access);
			atomic_fetch_sub(&target->threads, 1);
			if (err)
				*err = Crispr_cn_EDEAD;
			return false;
		}
	}
	mtx_unlock(&target->access);
	atomic_fetch_sub(&target->threads, 1);
	return true;
}

DLL_PUBLIC bool Crispr_f_sema_unlock(Crispr_t_Sema* target, Crispr_tn_Errno* err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	char lock = crispr_f_sema_statlock(target);
	if (lock & (Crispr_cn_Sema_TERM | Crispr_cn_Sema_KILL | Crispr_cn_Sema_DEAD)) {
		crispr_f_sema_statrel(target, lock);
		if (err)
			*err = Crispr_cn_ESTALE;
		return false;
	}
	{
		Crispr_t_uint count = atomic_fetch_add(&target->threads, 1);
		(void)count;
		assert(count < UINT_MAX);
	}
	crispr_f_sema_statrel(target, lock);
	{
		int stat = mtx_lock(&target->access);
		(void)stat;
		assert(stat == thrd_success);
	}
	if (target->limit > 0) {
		if (target->schedptr) {
			cnd_signal(&target->schedptr->lock);
		} else if (target->counter < target->limit) {
			target->counter += 1;
		} else {
			if (err)
				*err = Crispr_cn_EAGAIN;
			mtx_unlock(&target->access);
			atomic_fetch_sub(&target->threads, 1);
			return false;
		}
	}
	mtx_unlock(&target->access);
	atomic_fetch_sub(&target->threads, 1);
	return true;
}

DLL_PUBLIC bool Crispr_f_sema_destroy(Crispr_t_Sema* target, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	char lock = crispr_f_sema_statlock(target);
	if (lock == (Crispr_cn_Sema_KILL | Crispr_cn_Sema_DEAD)) {
		crispr_f_sema_statrel(target, lock);
		if (err)
			*err = Crispr_cn_ESTALE;
		return false;
	}
	{
		Crispr_t_uint count = atomic_fetch_add(&target->threads, 1);
		(void)count;
		assert(count < UINT_MAX);
	}
	crispr_f_sema_statrel(target, Crispr_cn_Sema_KILL);
	mtx_lock(&target->access);
	for (Crispr_t_SemSched* ptr = target->schedptr; ptr != Crispr_M_NULL; ptr = ptr->next) {
		Crispr_t_Sema* loaded = atomic_exchange(&ptr->parent, Crispr_M_NULL);
		if (loaded == &crispr_c_schedbusy) {
			int stat = cnd_signal(&ptr->lock);
			(void)stat;
			assert(stat == thrd_success);
		} else if (loaded != Crispr_M_NULL) {
			atomic_store(&ptr->parent, Crispr_M_NULL);
			cnd_destroy(&ptr->lock);
		}
	}
	target->schedptr = Crispr_M_NULL;
	mtx_unlock(&target->access);
	while (atomic_load(&target->threads) > 1) {
		mtx_lock(&target->access);
		mtx_unlock(&target->access);
	}
	mtx_destroy(&target->access);
	crispr_f_sema_statxchng(target, Crispr_cn_Sema_DEAD);
	return true;
}

DLL_PUBLIC bool Crispr_f_sema_schedinit(Crispr_t_SemSched* restrict dest, Crispr_t_Sema* src, bool term, Crispr_tn_Errno* restrict err) {
	if (err)
		*err = Crispr_cn_ENOERR;
	char lock = crispr_f_sema_statlock(src);
	if (lock & (Crispr_cn_Sema_TERM | Crispr_cn_Sema_KILL | Crispr_cn_Sema_IDLE)) {
		crispr_f_sema_statrel(src, lock);
		if (err)
			*err = Crispr_cn_ESTALE;
		return false;
	}
	{
		Crispr_t_uint count = atomic_fetch_add(&src->threads, 1);
		(void)count;
		assert(count < UINT_MAX);
	}
	crispr_f_sema_statrel(src, lock);
	atomic_fetch_sub(&src->threads, 1);
	return true;
}

DLL_RESTORE
