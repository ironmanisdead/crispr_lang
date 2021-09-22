#pragma once
#include "predefs.h"
#include <threads.h>

DLL_HIDE

typedef union {
	Crispr_Schar sbyte; //signed byte
	Crispr_Uchar ubyte; //unsigned byte
	short smore; //signed short
	Crispr_Ushort umore; //unsigned short
	int sint; //signed int
	Crispr_Uint uint; //unsigned int
	Crispr_S64 slong; //signed long
	Crispr_U64 ulong; //unsigned long
	Crispr_Size siz; //size type
	float flt; //single-precision float type
	double dbl; //double-precision float type
	void* mptr; //pointer to mutable memory
	const void* cptr; //pointer to constant memory
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
	Crispr_Word ret; //return value register
	Crispr_Word va0; //return register/first argument
	Crispr_Word va1; //second argument register
	Crispr_Word va2; //third argument register
	Crispr_Word va3; //fourth argument register
	Crispr_Word va4; //fifth argument register
	Crispr_Word va5; //sixth argument register
	struct {
		char zero : 1; //zero flag (set if result is zero)
		char carry : 1; //carry flag (set if overflow occurs)
		char adj : 1; //adjust flag (sets if upper nibble of lowest byte in destination has changed)
		char sign : 1; //sign flag (set if result is negative)
		char dir : 1; //direction flag for string processing (set if backwards, unset if forwards)
		char parity : 1; //parity (set if even, unset if odd)
		char brk : 1; //interrupt flag (set if interrupts are enabled)
	} flags;
	Crispr_Stack* stack; //holds the pointer to the current stack context
} Crispr_VM;

DLL_PUBLIC bool Crispr_stackInit(Crispr_Stack* restrict stack, const char* restrict codeseg,
		Crispr_Size len, Crispr_Errno* restrict err);

#define Crispr_vm(stck) ((Crispr_VM){ .flags = { .brk = 0, .dir = 0 }, .stack = stck })

typedef enum {
	CRISPR_VMOP_NOOP, //NO-op: read until next instruction
	CRISPR_VMOP_LOCK, //lock: make sure next instruction is not executed with any other locked instructions
	CRISPR_VMOP_HALT, //halt: stop the VM
	CRISPR_VMOP_MOVE, //move: move one thing into another
	CRISPR_VMOP_XCHG, //exchange: swap two values
	CRISPR_VMOP_XCHGCMP, //compare and exchange: swap dest with source if equal to comparison
	CRISPR_VMOP_AND, //does a bitwise "and" of two operands
	CRISPR_VMOP_OR, //does a bitwise "or" of two operands
	CRISPR_VMOP_XOR, //does a bitwise "xor" of two operands
	CRISPR_VMOP_ADD, //adds two operands together
	CRISPR_VMOP_SUB, //subtracts two operands from each other
	CRISPR_VMOP_MUL, //multiplies two operands together
	CRISPR_VMOP_DIV, //divides two operands by each other
	CRISPR_VMOP_SIZE, //re-sizes the stack according to the first operand (Crispr_Size assumed)
	CRISPR_VMOP_PUSH, //pushes value into stack, and resizes if nessecary
	CRISPR_VMOP_POP, //pops last pushed value out of stack
	CRISPR_VMOP_FRAME, //sets call frame of function
	CRISPR_VMOP_CALL, //calls another function
	CRISPR_VMOP_RET, //returns from function
} Crispr_VmOp; //VmOp describes the type of operation (or prefix)

typedef enum {
	CRISPR_VMSZ_BYTE, //byte-sized
	CRISPR_VMSZ_SHRT, //short-int sized
	CRISPR_VMSZ_INTG, //integer sized
	CRISPR_VMSZ_FLOT, //single-precision float sized
	CRISPR_VMSZ_DUBL, //double precision float sized
	CRISPR_VMSZ_SIZE, //size-type sized
	CRISPR_VMSZ_PNTR, //pointer-sized
} Crispr_VmSz; //VmSz describes operand size for certain operations

typedef enum {
	CRISPR_VMLD_MEM, //memory operand
	CRISPR_VMLD_STK, //stack offset operand
	CRISPR_VMLD_FRM, //frame offset operand
	CRISPR_VMLD_FLG, //specific flag offset
	CRISPR_VMLD_VA0, //return/first argument register
	CRISPR_VMLD_VA1, //second argument register
	CRISPR_VMLD_VA2, //third argument register
	CRISPR_VMLD_VA3, //fourth argument register
	CRiSPR_VMLD_VA4, //fifth argument register
	CRISPR_VMLD_VA5, //sixth argument register
} Crispr_VmLd; //VmLd describes the location (or load) of the operand

DLL_PUBLIC bool Crispr_runVM(Crispr_VM* restrict vm, Crispr_Size exec, Crispr_Errno* restrict err);

DLL_RESTORE
