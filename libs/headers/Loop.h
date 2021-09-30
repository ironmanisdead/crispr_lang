#pragma once
#include "predefs.h"

DLL_HIDE

typedef enum Dll_Enum {
	CRISPR_LOOP_FAIL, //loop failed at certain iteration
	CRISPR_LOOP_CONT, //loop is not yet done
	CRISPR_LOOP_DONE, //loop has finished successfully
} Crispr_LoopStat;

typedef Crispr_LoopStat (*Crispr_LoopStr)(const char* restrict str, Crispr_Size off, Crispr_Errno* restrict err); //string iterator

DLL_RESTORE
