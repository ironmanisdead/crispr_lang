#pragma once
#include "predefs.h"
#include <threads.h>

DLL_HIDE

typedef union {
	Crispr_Schar sbyte;
	Crispr_Uchar ubyte;
	short smore;
	Crispr_Ushort umore;
	int sint;
	Crispr_Uint uint;
	Crispr_S64 slong;
	Crispr_U64 ulong;
	Crispr_Size siz;
	double dbl;
	void* mptr;
	const void* cptr;
} Crispr_Word;

typedef struct _Crispr_Fixed {
	mtx_t airlock; //locks access to next Fixed object
	Crispr_Size len; //holds allocation size
	struct _Crispr_Fixed* next; //holds pointer to next Fixed object
	char data[]; //holds object (possibly with alignment padding)
} Crispr_Fixed;

#ifdef __GNUC__
 #pragma GCC poison _Crispr_Fixed
#endif

typedef struct {
	mtx_t lock; //used for non-owner to read/write, and for owner to reallocate
	Crispr_Size len; //current allocated length of stack
	char* data; //pointer to stack origin
	const char* restrict code; //instruction (code) pointer
	Crispr_Fixed* fixed; //pointer to linked list of Fixed objects
	Crispr_Size frame; //function call frame offset
	Crispr_Size end; //stack ending offset (where push puts things, and pop pops things)
} Crispr_Stack;

typedef struct {
	Crispr_Word retval;
	Crispr_Word src;
	Crispr_Word dst;
	Crispr_Stack* stack;
} Crispr_VM;

DLL_PUBLIC bool Crispr_stackInit(Crispr_Stack* restrict stack, const char* restrict codeseg,
		Crispr_Size len, Crispr_Errno* restrict err);

DLL_RESTORE
