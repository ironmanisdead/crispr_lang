#pragma once
#include "predefs.h"

DLL_HIDE

#define Crispr_M_LOCK_NONE 0b000000
#define Crispr_M_LOCK_SPEC 0b000001
#define Crispr_M_LOCK_USED 0b000010
#define Crispr_M_LOCK_WAIT 0b000011
#define Crispr_M_LOCK_UINT 0b000100
#define Crispr_M_LOCK_SINT 0b000101
#define Crispr_M_LOCK_FLOT 0b001000
#define Crispr_M_LOCK_DBLE 0b001001
#define Crispr_M_LOCK_CPTR 0b010000
#define Crispr_M_LOCK_MPTR 0b010001
#define Crispr_M_LOCK_RPTR 0b100000
#define Crispr_M_LOCK_RSTO 0b100001

typedef struct {
	_Atomic char flags;
	union {
		int plain;
		unsigned positive;
		float flot;
		double dbl;
		struct {
			Crispr_t_size len;
			union {
				const void* view;
				void* edit;
			};
		} entry;
	};
} Crispr_t_Lock;

DLL_PUBLIC void Crispr_f_lock_init(Crispr_t_Lock* restrict object);

DLL_PUBLIC bool Crispr_f_lock_alloc(volatile Crispr_t_Lock* obj, Crispr_t_size size, bool overwrite, Crispr_tn_Errno* restrict err);

DLL_RESTORE
