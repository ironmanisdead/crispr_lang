#pragma once
#include "predefs.h"

DLL_HIDE

#define CRISPR_LOCK_NONE 0b000000
#define CRISPR_LOCK_SPEC 0b000001
#define CRISPR_LOCK_USED 0b000010
#define CRISPR_LOCK_WAIT 0b000011
#define CRISPR_LOCK_UINT 0b000100
#define CRISPR_LOCK_SINT 0b000101
#define CRISPR_LOCK_FLOT 0b001000
#define CRISPR_LOCK_DBLE 0b001001
#define CRISPR_LOCK_CPTR 0b010000
#define CRISPR_LOCK_MPTR 0b010001
#define CRISPR_LOCK_RPTR 0b100000
#define CRISPR_LOCK_RSTO 0b100001

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
