#pragma once
#include "Loop.h"
#include <threads.h>

DLL_HIDE

typedef struct _Crispr_VmStk Crispr_VmStk;

typedef union {
	Crispr_Schar sbyte; //signed byte
	Crispr_Uchar ubyte; //unsigned byte
	short sshrt; //signed short
	Crispr_Ushort ushrt; //unsigned short
	int sint; //signed int
	Crispr_Uint uint; //unsigned int
	Crispr_S64 slong; //signed long
	Crispr_U64 ulong; //unsigned long
	Crispr_Size size; //memory size type (unsigned)
	Crispr_Off off; //memory offset type (signed)
	float flt; //single-precision float type
	double dbl; //double-precision float type
	char* ptr; //pointer to memory
	const char* cptr; //pointer to constant memory
	bool (*api)(const Crispr_VmStk*); //make api call
} Crispr_VmWrd;

typedef enum {
	CRISPR_VMSZ_PNTR, //pointer type
	CRISPR_VMSZ_BYTE, //byte type
	CRISPR_VMSZ_SHRT, //short int
	CRISPR_VMSZ_INTG, //plain int
	CRISPR_VMSZ_LONG, //64-bit int sized
	CRISPR_VMSZ_FLOT, //single-precision floating point
	CRISPR_VMSZ_DUBL, //double precision floating point
	CRISPR_VMSZ_SIZE, //size type
	CRISPR_VMSZ_WORD, //word sized
} Crispr_VmSz;

#define CRISPR_VMTY_SIGN 0x4000
#define CRISPR_VMTY_DREF 0x8000
#define Crispr_vmSz(typ) (Crispr_VmSz)(typ & 0x00FF)
#define Crispr_vmTy(siz) (Crispr_VmTy)(siz)

typedef Crispr_Ushort Crispr_VmTy;

typedef struct _Crispr_VmFixObj {
	mtx_t airlock; //locks access to next Fixed object
	Crispr_Size len; //holds allocation size
	struct _Crispr_VmFixObj* next; //holds pointer to next Fixed object
	char data[]; //holds object (possibly with alignment padding)
} Crispr_VmFixObj;

typedef struct _Crispr_VmCdSeg {
	Crispr_Uint ver; //Symbol version information
	Crispr_Size len; //Code length information (zero if C code)
	const char* code; //Segment base pointer
	const char* sym; //Symbol header
	struct _Crispr_VmCdSeg* next; //next code segment
	struct _Crispr_VmCdSeg* back; //first code segment in version (unset if start of version)
	struct _Crispr_VmCdSeg* zero; //Starting code segment (set if end of version)
} Crispr_VmCdSeg;

typedef struct _Crispr_VmNameSpace {
	Crispr_Uint id; //namespace id/descriptor
	Crispr_VmCdSeg* base; //base (lowest version) codeseg for namespace
	struct _Crispr_VmNameSpace* next; //next namespace
} Crispr_VmNameSpace;

#ifdef __GNUC__
 #pragma GCC poison _Crispr_VmFixObj
 #pragma GCC poison _Crispr_VmCdSeg
 #pragma GCC poison _Crispr_VmNameSpace
#endif

typedef struct {
	mtx_t lock; //locks the VM when a stack is using it
	Crispr_VmStk* stack; //pointer to the first loaded stack
	Crispr_VmWrd regs[6];
	short flags; //flags register
	const Crispr_VmNameSpace* restrict base; //base namespace for symbol loading
} Crispr_VM;

struct _Crispr_VmStk {
	Crispr_VM* vm; //parent VM context
	Crispr_VmStk* next; //sibling stack
	Crispr_VmStk** from; //pointer from previous stack
	const Crispr_VmCdSeg* segc; //current code segment
	Crispr_Size sp; //instruction starting offset
	Crispr_Size rp; //internal instruction read offset
	Crispr_VmWrd cal[6]; //caller-save registers
	Crispr_VmWrd imm[6]; //immediate registers
	mtx_t lock; //used for non-owner to read/write, and for owner to reallocate
	Crispr_Size len; //current allocated length of stack
	char* origin; //stack origin
	Crispr_VmFixObj* fixed; //pointer to linked list of Fixed objects
	Crispr_Off frame; //current function call frame offset
	Crispr_Off call; //callframe for next function to call (0 if not set, negative is considered errorneous)
	Crispr_Off end; //stack ending offset (where push puts things, and pop pops things)
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_VmStk
#endif

#define CRISPR_VMFL_ZF 0x01 //zero flag
#define CRISPR_VMFL_CF 0x02 //carry flag
#define CRISPR_VMFL_AD 0x04 //adjust flag
#define CRISPR_VMFL_SF 0x08 //sign flag
#define CRISPR_VMFL_PF 0x10 //parity flag
#define CRISPR_VMFL_DF 0x20 //direction flag
#define CRISPR_VMFL_BF 0x40 //interrupt flag
#define CRISPR_VMFL_HF 0x80 //heap flag
typedef short Crispr_VmFl;

DLL_PUBLIC bool Crispr_vmStackInit(Crispr_VmStk* restrict stack, 
		Crispr_VM* vm, Crispr_VmCdSeg* restrict ns, Crispr_Size len,
		Crispr_Size code, Crispr_Errno* restrict err);

DLL_PUBLIC bool Crispr_vmInit(Crispr_VM* vm, const Crispr_VmNameSpace* ns, Crispr_Errno* err);

typedef enum Dll_Enum {
	CRISPR_VMOP_NOOP, //NO-op: read until next instruction
	CRISPR_VMOP_LOCK, //lock: make sure next instruction is not executed with any other locked instructions
	CRISPR_VMOP_STOP, //stop: defer execution of this code until Crispr_vmRun is called on it again
	CRISPR_VMOP_HALT, //halt: prevent this code from running any more
	CRISPR_VMOP_MOV, //move: move one thing into another
	CRISPR_VMOP_POS, //position: get effective address of offset
	CRISPR_VMOP_XCH, //exchange: swap two values
	CRISPR_VMOP_XCM, //compare and exchange: swap dest with source if equal to comparison
	CRISPR_VMOP_NOT, //does a bitwise "not" of one operand
	CRISPR_VMOP_AND, //does a bitwise "and" of two operands
	CRISPR_VMOP_IOR, //does a bitwise inclusive "or" of two operands
	CRISPR_VMOP_XOR, //does a bitwise exclusive "or" of two operands
	CRISPR_VMOP_ADD, //adds two operands together
	CRISPR_VMOP_SUB, //subtracts two operands from each other
	CRISPR_VMOP_MUL, //multiplies two operands together
	CRISPR_VMOP_DIV, //divides two operands by each other
	CRISPR_VMOP_CNV, //convert: converts one numerical type into another
	CRISPR_VMOP_SIZ, //re-sizes the stack according to the first operand (Crispr_Size)
	CRISPR_VMOP_PUT, //pushes value into stack, and resizes if nessecary
	CRISPR_VMOP_POP, //pops last pushed value out of stack
	CRISPR_VMOP_FRM, //sets call frame of function
	CRISPR_VMOP_JMP, //jumps with more conditions specified
	CRISPR_VMOP_CAL, //calls another function
	CRISPR_VMOP_RET, //returns from function
	CRISPR_VMOP_HOP, //heap operation
} Crispr_VmOp; //VmOp describes the type of operation (or prefix)

typedef enum Dll_Enum {
	CRISPR_VMHP_FLP, //change from global to local, and vice versa
	CRISPR_VMHP_GLO, //change heap to global
	CRISPR_VMHP_LOC, //change heap to local
	CRISPR_VMHP_NEW, //insert new object in heap (Off, Alignment)
	CRISPR_VMHP_DEL, //deallocate object in heap
	CRISPR_VMHP_CHG, //move over certain amount of allocations (Off)
	CRISPR_VMHP_SET, //move certain amount of allocations from base pointer (Off)
	CRISPR_VMHP_SIZ, //resize heap object (can cause object relocation) (Size)
} Crispr_VmHp; //Heap oPerator: operations that affect the heap

typedef enum Dll_Enum {
	CRISPR_VMLD_LIT, //literal value
	CRISPR_VMLD_OFF, //load offset
	CRISPR_VMLD_FLG, //load flag set
	CRISPR_VMLD_REG, //load register
} Crispr_VmLd; //VmLd describes the location (or load) of the operand

typedef enum Dll_Enum {
	CRISPR_VMOF_MEM, //memory offset
	CRISPR_VMOF_OBJ, //current heap object offset
	CRISPR_VMOF_STK, //stack offset
	CRISPR_VMOF_SYM, //symbol offset
	CRISPR_VMOF_FRM, //frame offset
	CRISPR_VMOF_CAL, //call frame offset
	CRISPR_VMOF_COD, //code pointer offset
	CRISPR_VMOF_CUR, //instruction pointer offset
} Crispr_VmOf; //VmOf describes an offset type

typedef struct {
	Crispr_VmFl flags;
	struct {
		mtx_t* lock;
		char *addr;
	} ptr;
} Crispr_VmRef;

DLL_PUBLIC Crispr_LoopStat Crispr_vmRun(Crispr_VmStk* restrict stack, Crispr_Size exec, Crispr_Errno* restrict err);

DLL_RESTORE
