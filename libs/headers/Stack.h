#pragma once
#include "Sema.h"

DLL_HIDE

typedef struct {
	Crispr_t_ulong id; //0 if invalid
	Crispr_t_Sema sem;
} Crispr_t_Sema_Node;

typedef struct {
	Crispr_t_State* state;
	Crispr_t_Sema stacklock;
	Crispr_t_size lockmem;
	Crispr_t_size lockent;
	Crispr_t_Sema_Node* objlocks;
	Crispr_t_size objmem;
	Crispr_t_size objent;
	char* objects;
} Crispr_t_Stack;

DLL_PUBLIC bool Crispr_f_stack_init(Crispr_t_Stack* restrict, Crispr_tn_Errno* restrict);

DLL_RESTORE
