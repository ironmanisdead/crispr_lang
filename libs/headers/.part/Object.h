#pragma once
#include "../Class.h"

DLL_HIDE

typedef struct {
	Crispr_ClsRefLck type;
	char data[];
} Crispr_Object;

DLL_RESTORE
