#pragma once
#include "Stack.h"
#include <threads.h>

DLL_HIDE

typedef struct {
	volatile _Atomic bool valid;
	Crispr_Ulong id;
	thrd_t internal;
	volatile _Atomic bool done;
	volatile Crispr_Stack* ret;
} Crispr_Thread;

DLL_PUBLIC bool Crispr_thread_init(Crispr_Thread* restrict dest,
		void (*callback)(Crispr_Stack* ret, const Crispr_Stack* args),
		const Crispr_Stack* args, Crispr_Errno* restrict stat);

DLL_PUBLIC Crispr_Ulong Crispr_thread_id();

DLL_RESTORE
