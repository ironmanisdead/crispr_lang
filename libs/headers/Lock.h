#pragma once
#include "predefs.h"

DLL_HIDE

#define CRISPR_LOCK_NONE 0x000000
#define CRISPR_LOCK_SPEC 0x000001
#define CRISPR_LOCK_USED 0x000002
#define CRISPR_LOCK_WAIT 0x000003
#define CRISPR_LOCK_UINT 0x000004
#define CRISPR_LOCK_SINT 0x000005
#define CRISPR_LOCK_FLOT 0x000008
#define CRISPR_LOCK_DBLE 0x000009
#define CRISPR_LOCK_CPTR 0x000010
#define CRISPR_LOCK_MPTR 0x000011
#define CRISPR_LOCK_RPTR 0x000020
#define CRISPR_LOCK_RSTO 0x000021

typedef struct {
	_Atomic char flags;
	union {
		int plain;
		unsigned positive;
		float flot;
		double dbl;
		struct {
			Crispr_Size len;
			union {
				const void* view;
				void* edit;
			};
		} entry;
	};
} Crispr_Lock;

DLL_PUBLIC void Crispr_lock_init(Crispr_Lock* restrict object);

DLL_PUBLIC bool Crispr_lock_alloc(volatile Crispr_Lock* obj, Crispr_Size size, bool overwrite, Crispr_Errno* restrict err);

DLL_RESTORE
