#pragma once
#include "Loop.h"
#include <threads.h>

DLL_HIDE

typedef struct _Crispr_VmStack Crispr_VmStack;

typedef union {
	Crispr_Schar sbyte; //signed byte
	Crispr_Uchar ubyte; //unsigned byte
	short sshrt; //signed short
	Crispr_Ushort ushrt; //unsigned short
	int sint; //signed int
	Crispr_Uint uint; //unsigned int
	Crispr_S64 slong; //signed long
	Crispr_U64 ulong; //unsigned long
	Crispr_Size size; //size type
	Crispr_Off off; //offset type
	float flt; //single-precision float type
	double dbl; //double-precision float type
	char* ptr; //pointer to memory
	const char* cptr; //pointer to constant memory
	bool (*api)(const Crispr_VmStack*); //make api call
} Crispr_VmWord;

typedef struct _Crispr_VmFixObj {
	mtx_t airlock; //locks access to next Fixed object
	Crispr_Size len; //holds allocation size
	struct _Crispr_VmFixObj* next; //holds pointer to next Fixed object
	char data[]; //holds object (possibly with alignment padding)
} Crispr_VmFixObj;

typedef struct _Crispr_VmCodeSeg {
	Crispr_Uint ver; //Symbol version information
	Crispr_Size len; //Code length information (zero if C library)
	const char* code; //Code pointer
	const char* sym; //Symbol header information
	struct _Crispr_VmCodeSeg* next; //next code segment
} Crispr_VmCodeSeg;

typedef struct _Crispr_VmNameSpace {
	Crispr_Uint id; //namespace id/descriptor
	Crispr_VmCodeSeg* base; //base codeseg
	struct _Crispr_VmNameSpace* next; //next namespace
} Crispr_VmNameSpace;

#ifdef __GNUC__
 #pragma GCC poison _Crispr_VmFixObj
 #pragma GCC poison _Crispr_VmCodeSeg
 #pragma GCC poison _Crispr_VmNameSpace
#endif

typedef struct {
	Crispr_VmWord regs[6]; //register set
} Crispr_VmReg; //register set for VM

typedef struct {
	mtx_t lock; //locks the VM when a stack is using it
	Crispr_VmStack* stack; //pointer to the first loaded stack
	Crispr_VmReg regs; //VM-global registers
	short flags; //flags register
	const Crispr_VmNameSpace* restrict base; //base namespace for symbol loading
} Crispr_VM;

struct _Crispr_VmStack {
	Crispr_VM* vm; //parent VM context
	Crispr_VmStack* next; //sibling stack
	Crispr_VmStack** from; //pointer from previous stack
	const Crispr_VmCodeSeg* base_seg; //stack call segment in namespace
	const Crispr_VmCodeSeg* cur_seg; //current call segment in code
	Crispr_Size ip; //code offset
	Crispr_VmReg cal; //caller-save registers
	Crispr_VmReg imm; //immediate registers
	mtx_t lock; //used for non-owner to read/write, and for owner to reallocate
	Crispr_Size len; //current allocated length of stack
	char* origin; //stack origin
	Crispr_VmFixObj* fixed; //pointer to linked list of Fixed objects
	Crispr_Size frame; //function call frame offset
	Crispr_Size end; //stack ending offset (where push puts things, and pop pops things)
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_VmStack
#endif

#define CRISPR_VMFL_ZF 0x01 //zero flag
#define CRISPR_VMFL_CF 0x02 //carry flag
#define CRISPR_VMFL_AD 0x04 //adjust flag
#define CRISPR_VMFL_SF 0x08 //sign flag
#define CRISPR_VMFL_PF 0x10 //parity flag
#define CRISPR_VMFL_DF 0x20 //direction flag
#define CRISPR_VMFL_BF 0x40 //interrupt flag

DLL_PUBLIC bool Crispr_vmStackInit(Crispr_VmStack* restrict stack, Crispr_VM* vm, Crispr_Size init,
		Crispr_Size len, Crispr_Errno* restrict err);

DLL_PUBLIC bool Crispr_vmInit(Crispr_VM* vm, const Crispr_VmNameSpace* ns, Crispr_Errno* err);

typedef enum Dll_Enum {
	CRISPR_VMOP_NOOP, //NO-op: read until next instruction
	CRISPR_VMOP_LOCK, //lock: make sure next instruction is not executed with any other locked instructions
	CRISPR_VMOP_STOP, //stop: defer execution of this code until Crispr_vmRun is called on it again
	CRISPR_VMOP_HALT, //halt: prevent this code from running any more
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
} Crispr_VmOp; //VmOp describes the type of operation (or prefix)

typedef enum Dll_Enum {
	CRISPR_VMSZ_NPTR, //void pointer (internal)
	CRISPR_VMSZ_NSIZ, //offset parameter type (internal)
	CRISPR_VMSZ_BYTE, //byte-sized
	CRISPR_VMSZ_SHRT, //short-int sized
	CRISPR_VMSZ_INTG, //integer sized
	CRISPR_VMSZ_LONG, //long long int sized
	CRISPR_VMSZ_FLOT, //single-precision float sized
	CRISPR_VMSZ_DUBL, //double precision float sized
	CRISPR_VMSZ_SIZE, //size-type sized
	CRISPR_VMSZ_PNTR, //pointer-sized
	CRISPR_VMSZ_WORD, //size as one word
} Crispr_VmSz; //VmSz describes operand size for certain operations

typedef enum Dll_Enum {
	CRISPR_VMLD_LIT, //literal value
	CRISPR_VMLD_OFF, //load offset
	CRISPR_VMLD_FLG, //load flag
	CRISPR_VMLD_REG_VM, //VM global register
	CRISPR_VMLD_REG_RA, //caller-save register
	CRISPR_VMLD_REG_IM, //intermediate-value register
} Crispr_VmLd; //VmLd describes the location (or load) of the operand

typedef enum Dll_Enum {
	CRISPR_VMOF_MEM, //memory offset
	CRISPR_VMOF_STK, //stack offset
	CRISPR_VMOF_SYM, //symbol offset
	CRISPR_VMOF_FRM, //frame offset
	CRISPR_VMOF_COD, //code pointer offset
	CRISPR_VMOF_CUR, //instruction pointer offset
} Crispr_VmOf; //runs with flag offset

DLL_PUBLIC Crispr_LoopStat Crispr_vmRun(Crispr_VmStack* restrict stack, Crispr_Size exec, Crispr_Errno* restrict err);

DLL_RESTORE
