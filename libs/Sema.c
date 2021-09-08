#include "headers/Sema.h"
#include "headers/.part/Sema.h"
#include "headers/Memory.h"
#include "headers/Errno.h"
#include <stdatomic.h>
#include <limits.h>
#include <assert.h>

DLL_HIDE

static Crispr_Sema crispr_c_schedbusy; //constant to tell semaphore busy status

DLL_PUBLIC bool Crispr_sema_init(Crispr_Sema* restrict target, Crispr_Ulong limit, Crispr_Errno* restrict err) {
	atomic_init(&target->status, CRISPR_SEMA_DEAD);
	target->threads = 0;
	if (mtx_init(&target->access, mtx_plain) == thrd_error) {
		if (err)
			*err = CRISPR_ERRUNKNOWN;
		return false;
	}
	if (limit > 0) {
		atomic_store(&target->status, CRISPR_SEMA_IDLE);
		target->limit = limit;
		target->counter = 1;
		target->schedptr = CRISPR_NULL;
		return true;
	}
	{
		int i = cnd_init(&target->waiter);
		if (i == thrd_error) {
			mtx_destroy(&target->access);
			if (err)
				*err = CRISPR_ERRUNKNOWN;
			return false;
		} else if (i == thrd_nomem) {
			mtx_destroy(&target->access);
			if (err)
				*err = CRISPR_ERRNOMEM;
			return false;
		}
	}
	target->limit = 0;
	atomic_store(&target->status, CRISPR_SEMA_IDLE);
	return true;
}

static char crispr_sema_statlock(Crispr_Sema* target) {
	char stat;
	while (true) {
		stat = atomic_exchange(&target->status, CRISPR_SEMA_USED);
		if (stat != CRISPR_SEMA_USED) {
			return stat;
		}
	}
}

static void crispr_sema_statrel(Crispr_Sema* target, char stat) {
	char status = atomic_exchange(&target->status, stat);
	(void)status;
	assert(status == CRISPR_SEMA_USED);
}

static char crispr_sema_statload(Crispr_Sema* target) {
	char stat;
	while (true) {
		stat = atomic_exchange(&target->status, CRISPR_SEMA_USED);
		if (stat != CRISPR_SEMA_USED)
			break;
	}
	atomic_store(&target->status, stat);
	return stat;
}

static char crispr_sema_statxchng(Crispr_Sema* target, char stat) {
	char lock = crispr_sema_statlock(target);
	crispr_sema_statrel(target, stat);
	return lock;
}

static void crispr_sema_shift(Crispr_SemSched* rm) {
	cnd_destroy(&rm->lock);
	Crispr_SemSched* next = rm->next;
	Crispr_SemSched** prev = rm->prev;
	*prev = next;
	if (next)
		next->prev = prev;
	rm->parent = CRISPR_NULL;
}

static Crispr_SemSched** crispr_sema_latest(Crispr_Sema* target) {
	Crispr_SemSched** latest = &target->schedptr;
	Crispr_SemSched* current = *latest;
	while (current) {
		latest = &current->next;
		current = *latest;
	}
	return latest;
}

static char crispr_sema_time(struct timespec* restrict dest, const Crispr_Timer* restrict src) {
	if (src) {
		if ((src->clock == CRISPR_CLK_RELA) && (src->computed == 0)) {
			return 2;
		} else {
			Crispr_Timer time;
			assert(Crispr_timeConv(&time, src, CRISPR_CLK_UTC, CRISPR_TIME_NANOSECOND, CRISPR_NULL));
			dest->tv_sec = time.computed / CRISPR_TIME_SECOND;
			dest->tv_nsec = time.computed % CRISPR_TIME_SECOND;
			return 1;
		}
	} else {
		return 0;
	}
}

DLL_PUBLIC bool Crispr_sema_lock(Crispr_Sema* target, bool term, const Crispr_Timer* restrict wait, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	char lock = crispr_sema_statlock(target);
	if (lock & (CRISPR_SEMA_TERM | CRISPR_SEMA_KILL | CRISPR_SEMA_DEAD)) {
		crispr_sema_statrel(target, lock);
		if (err)
			*err = CRISPR_ERRSTALE;
		return false;
	}
	{
		Crispr_Uint count = target->threads++;
		(void)count;
		assert(count < UINT_MAX);
	}
	//statrel should be called after the emergency thread counter is incremented
	if (term) {
		crispr_sema_statrel(target, CRISPR_SEMA_TERM);
	} else {
		crispr_sema_statrel(target, CRISPR_SEMA_IDLE);
	}
	struct timespec timeconv;
	char present = crispr_sema_time(&timeconv, wait);
	{
		int stat = mtx_lock(&target->access);
		(void)stat;
		assert(stat == thrd_success);
	}
	if (target->limit > 0) {
		if (target->counter > 0) {
			target->counter -= 1;
			mtx_unlock(&target->access);
			--target->threads;
			return true;
		}
		Crispr_SemSched** latest = crispr_sema_latest(target);
		Crispr_SemSched waiter;
		atomic_init(&waiter.parent, &crispr_c_schedbusy);
		waiter.next = CRISPR_NULL;
		waiter.prev = latest;
		int i = cnd_init(&waiter.lock);
		if (i == thrd_error) {
			mtx_unlock(&target->access);
			--target->threads;
			if (err)
				*err = CRISPR_ERRUNKNOWN;
			return false;
		} else if (i == thrd_nomem) {
			mtx_unlock(&target->access);
			--target->threads;
			if (err)
				*err = CRISPR_ERRNOMEM;
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
			char load = crispr_sema_statlock(target);
			crispr_sema_shift(&waiter);
			mtx_unlock(&target->access);
			if ((load & CRISPR_SEMA_TERM) && (target->threads < 2)) {
				mtx_destroy(&target->access);
				crispr_sema_statrel(target, CRISPR_SEMA_DEAD);
			} else if (load & CRISPR_SEMA_KILL) {
				--target->threads;
				crispr_sema_statrel(target, load);
				if (err)
					*err = CRISPR_ERRDEAD;
				return false;
			}
			--target->threads;
			crispr_sema_statrel(target, load);
			if (err)
				*err = CRISPR_ERRTIMEDOUT;
			return false;
		} else {
			char load = crispr_sema_statlock(target);
			if (load & CRISPR_SEMA_KILL) {
				mtx_unlock(&target->access);
				--target->threads;
				crispr_sema_statrel(target, load);
				if (err)
					*err = CRISPR_ERRDEAD;
				return false;
			} else if ((load & CRISPR_SEMA_TERM) && (target->threads < 2)) {
				mtx_unlock(&target->access);
				mtx_destroy(&target->access);
				target->threads = 0;
				crispr_sema_statrel(target, CRISPR_SEMA_DEAD);
				return true;
			} else {
				crispr_sema_statrel(target, load);
				crispr_sema_shift(&waiter);
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
		char lock = crispr_sema_statload(target);
		if (result == thrd_timedout) {
			mtx_unlock(&target->access);
			--target->threads;
			if (err)
				*err = CRISPR_ERRTIMEDOUT;
			return false;
		} else if (lock == CRISPR_SEMA_KILL) {
			mtx_unlock(&target->access);
			--target->threads;
			if (err)
				*err = CRISPR_ERRDEAD;
			return false;
		}
	}
	mtx_unlock(&target->access);
	--target->threads;
	return true;
}

DLL_PUBLIC bool Crispr_sema_unlock(Crispr_Sema* target, Crispr_Errno* err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	char lock = crispr_sema_statlock(target);
	if (lock & (CRISPR_SEMA_TERM | CRISPR_SEMA_KILL | CRISPR_SEMA_DEAD)) {
		crispr_sema_statrel(target, lock);
		if (err)
			*err = CRISPR_ERRSTALE;
		return false;
	}
	{
		Crispr_Uint count = target->threads++;
		(void)count;
		assert(count < UINT_MAX);
	}
	crispr_sema_statrel(target, lock);
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
				*err = CRISPR_ERRAGAIN;
			mtx_unlock(&target->access);
			--target->threads;
			return false;
		}
	}
	mtx_unlock(&target->access);
	--target->threads;
	return true;
}

DLL_PUBLIC bool Crispr_sema_destroy(Crispr_Sema* target, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	char lock = crispr_sema_statlock(target);
	if (lock == (CRISPR_SEMA_KILL | CRISPR_SEMA_DEAD)) {
		crispr_sema_statrel(target, lock);
		if (err)
			*err = CRISPR_ERRSTALE;
		return false;
	}
	{
		Crispr_Uint count = target->threads++;
		(void)count;
		assert(count < UINT_MAX);
	}
	crispr_sema_statrel(target, CRISPR_SEMA_KILL);
	mtx_lock(&target->access);
	for (Crispr_SemSched* ptr = target->schedptr; ptr != CRISPR_NULL; ptr = ptr->next) {
		Crispr_Sema* loaded = atomic_exchange(&ptr->parent, CRISPR_NULL);
		if (loaded == &crispr_c_schedbusy) {
			int stat = cnd_signal(&ptr->lock);
			(void)stat;
			assert(stat == thrd_success);
		} else if (loaded != CRISPR_NULL) {
			atomic_store(&ptr->parent, CRISPR_NULL);
			cnd_destroy(&ptr->lock);
		}
	}
	target->schedptr = CRISPR_NULL;
	mtx_unlock(&target->access);
	while (target->threads > 1) {
		mtx_lock(&target->access);
		mtx_unlock(&target->access);
	}
	mtx_destroy(&target->access);
	crispr_sema_statxchng(target, CRISPR_SEMA_DEAD);
	return true;
}

DLL_PUBLIC bool Crispr_sema_schedInit(Crispr_SemSched* restrict dest, Crispr_Sema* src, bool term, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	char lock = crispr_sema_statlock(src);
	if (lock & (CRISPR_SEMA_TERM | CRISPR_SEMA_KILL | CRISPR_SEMA_DEAD)) {
		crispr_sema_statrel(src, lock);
		if (err)
			*err = CRISPR_ERRSTALE;
		return false;
	}
	{
		Crispr_Uint count = src->threads++;
		(void)count;
		assert(count < UINT_MAX);
	}
	if (term)
		lock = CRISPR_SEMA_TERM;
	Crispr_SemSched** latest = crispr_sema_latest(src);
	int i = cnd_init(&dest->lock);
	if (i == thrd_error) {
		--src->threads;
		crispr_sema_statrel(src, lock);
		if (err)
			*err = CRISPR_ERRUNKNOWN;
		return false;
	} else if (i == thrd_nomem) {
		--src->threads;
		crispr_sema_statrel(src, lock);
		if (err)
			*err = CRISPR_ERRNOMEM;
		return false;
	}
	atomic_init(&dest->parent, src);
	dest->next = CRISPR_NULL;
	dest->prev = latest;
	*latest = dest;
	--src->threads;
	crispr_sema_statrel(src, CRISPR_SEMA_TERM);
	return true;
}

DLL_RESTORE
