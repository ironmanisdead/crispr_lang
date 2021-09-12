#pragma once
#include "predefs.h"

DLL_HIDE

#define CRISPR_LOCK_NONE 0x00
#define CRISPR_LOCK_SPEC 0x01
#define CRISPR_LOCK_USED 0x02
#define CRISPR_LOCK_WAIT 0x03
#define CRISPR_LOCK_UINT 0x04
#define CRISPR_LOCK_SINT 0x05
#define CRISPR_LOCK_FLOT 0x08
#define CRISPR_LOCK_DBLE 0x09
#define CRISPR_LOCK_CPTR 0x10
#define CRISPR_LOCK_MPTR 0x11
#define CRISPR_LOCK_RPTR 0x20
#define CRISPR_LOCK_RSTO 0x21

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
