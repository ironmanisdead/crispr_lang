#pragma once
#include "Stack.h"
#include <threads.h>

DLL_HIDE

typedef struct {
	volatile _Atomic bool valid;
	Crispr_t_ulong id;
	thrd_t internal;
	volatile _Atomic bool done;
	volatile Crispr_t_Stack* ret;
} Crispr_t_Thread;

DLL_PUBLIC bool Crispr_f_thread_init(Crispr_t_Thread* restrict dest,
		void (*callback)(Crispr_t_Stack* ret, const Crispr_t_Stack* args),
		const Crispr_t_Stack* args, Crispr_tn_Errno* restrict stat);

DLL_PUBLIC Crispr_t_ulong Crispr_f_thread_id();

DLL_RESTORE
