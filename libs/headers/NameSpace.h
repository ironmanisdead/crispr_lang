#pragma once
#include "predefs.h"

DLL_HIDE

typedef struct {
	const Crispr_NameSpace* ns;
	Crispr_SemSched lock;
} Crispr_NsRef;

Crispr_NameSpace* Crispr_nsCreate(const char* restrict name, const Crispr_NsRef* parent);

bool Crispr_nsGet(Crispr_NsRef* restrict dest, const char* restrict name);

DLL_RESTORE
