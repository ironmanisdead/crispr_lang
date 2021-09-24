#pragma once
#include "predefs.h"
#include <threads.h>

DLL_HIDE

typedef struct _Crispr_Stack Crispr_Stack;

typedef union {
	Crispr_Schar sbyte; //signed byte
	Crispr_Uchar ubyte; //unsigned byte
	short smore; //signed short
	Crispr_Ushort umore; //unsigned short
	int sint; //signed int
	Crispr_Uint uint; //unsigned int
	Crispr_S64 slong; //signed long
	Crispr_U64 ulong; //unsigned long
	Crispr_Size off; //offset type
	float flt; //single-precision float type
	double dbl; //double-precision float type
	void* ptr; //pointer to memory
	bool (*api)(const Crispr_Stack*); //make api call
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
	Crispr_Word regs[6]; //register set
} Crispr_VmReg; //register set for VM

struct _Crispr_Stack {
	Crispr_Size ip; //instruction pointer offset
	Crispr_VmReg cal; //caller-save registers
	Crispr_VmReg imm; //immediate registers
	mtx_t lock; //used for non-owner to read/write, and for owner to reallocate
	Crispr_Size len; //current allocated length of stack
	char* origin; //stack origin
	Crispr_Fixed* fixed; //pointer to linked list of Fixed objects
	Crispr_Size frame; //function call frame offset
	Crispr_Size end; //stack ending offset (where push puts things, and pop pops things)
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_Stack
#endif

typedef struct {
	Crispr_VmReg regs; //VM-global registers
	short flags; //flags register
	const char* restrict code; //code base pointer
	Crispr_Stack* stack; //holds the pointer to the current stack context
} Crispr_VM;

#define CRISPR_VMFL_ZF 0x01 //zero flag
#define CRISPR_VMFL_CF 0x02 //carry flag
#define CRISPR_VMFL_AD 0x04 //adjust flag
#define CRISPR_VMFL_SF 0x08 //sign flag
#define CRISPR_VMFL_PF 0x10 //parity flag
#define CRISPR_VMFL_DF 0x20 //direction flag
#define CRISPR_VMFL_BF 0x40 //interrupt flag

DLL_PUBLIC bool Crispr_stackInit(Crispr_Stack* restrict stack, Crispr_Size init,
		Crispr_Size len, Crispr_Errno* restrict err);

#define Crispr_vm(stck) ((Crispr_VM){ .flags = 0x00, .stack = stck })

#pragma push_macro("ENUM_PACK")
#ifdef __GNUC__
 #define ENUM_PACK __attribute__ ((__packed__))
#else
 #define ENUM_PACK
#endif

typedef enum ENUM_PACK {
	CRISPR_VMOP_NOOP, //NO-op: read until next instruction
	CRISPR_VMOP_LOCK, //lock: make sure next instruction is not executed with any other locked instructions
	CRISPR_VMOP_HALT, //halt: stop interpreting instructions from this code
	CRISPR_VMOP_STOP, //stop: interrupt interpreting instructions from this code
	CRISPR_VMOP_MOVE, //move: move one thing into another
	CRISPR_VMOP_POSI, //position: get effective address of offset
	CRISPR_VMOP_XCHG, //exchange: swap two values
	CRISPR_VMOP_XCMP, //compare and exchange: swap dest with source if equal to comparison
	CRISPR_VMOP_NOT, //does a bitwise "not" of one operand
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
	CRISPR_VMOP_JMP, //jumps with more conditions specified
	CRISPR_VMOP_CALL, //calls another function
	CRISPR_VMOP_RET, //returns from function
	CRISPR_VMOP_API, //calls api function
} Crispr_VmOp; //VmOp describes the type of operation (or prefix)

typedef enum ENUM_PACK {
	CRISPR_VMSZ_BYTE, //byte-sized
	CRISPR_VMSZ_SHRT, //short-int sized
	CRISPR_VMSZ_INTG, //integer sized
	CRISPR_VMSZ_FLOT, //single-precision float sized
	CRISPR_VMSZ_DUBL, //double precision float sized
	CRISPR_VMSZ_SIZE, //size-type sized
	CRISPR_VMSZ_PNTR, //pointer-sized
} Crispr_VmSz; //VmSz describes operand size for certain operations

typedef enum ENUM_PACK {
	CRISPR_VMLD_LIT, //literal value
	CRISPR_VMLD_OFF, //load offset
	CRISPR_VMLD_FLG, //load flag
	CRISPR_VMLD_REG_VM, //VM global register
	CRISPR_VMLD_REG_RA, //caller-save register
	CRISPR_VMLD_REG_IM, //intermediate-value register
} Crispr_VmLd; //VmLd describes the location (or load) of the operand

typedef enum ENUM_PACK {
	CRISPR_VMOF_MEM, //memory offset
	CRISPR_VMOF_STK, //stack offset
	CRISPR_VMOF_FRM, //frame offset
	CRISPR_VMOF_COD, //code pointer offset
} Crispr_VmOf; //runs with flag offset

DLL_PUBLIC bool Crispr_vmRun(Crispr_VM* restrict vm, Crispr_Size exec, Crispr_Errno* restrict err);

#pragma pop_macro("ENUM_PACK")

DLL_RESTORE
