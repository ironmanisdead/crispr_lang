#include "headers/VM.h"
#include "headers/Memory.h"
#include "headers/Error.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_stackInit(Crispr_Stack* restrict stack, Crispr_Size init, Crispr_Size len, Crispr_Errno* err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	if (len == 0)
		len = 16;
	stack->origin = Crispr_malloc(len);
	if (!stack->origin) {
		*err = CRISPR_ERR_NOMEM;
		return false;
	}
	stack->len = len;
	stack->ip = init;
	return true;
}

typedef struct {
	short flags; //holds flag type
	union {
		void* ptr;
		Crispr_Size off;
	};
} Crispr_VmRef;

static bool crispr_vm_offset(void* restrict* restrict res, Crispr_Stack* stack, Crispr_Errno* restrict err) {
}

static bool crispr_vmGetWord(Crispr_Word* restrict wrd, Crispr_VM* restrict vm, Crispr_VmSz size,
		bool offset, Crispr_Errno* restrict err) {
	Crispr_Stack* stack = vm->stack;
	switch (*(Crispr_VmLd*)&vm->code[stack->ip]) {
		case CRISPR_VMLD_LIT:
			stack->ip += sizeof(Crispr_VmLd);
			*wrd = *(Crispr_Word*)(&vm->code[stack->ip]);
			stack->ip += sizeof(Crispr_Word);
			return true;
		case CRISPR_VMLD_OFF:
			if (!offset) {
				if (err)
					*err = CRISPR_ERR_VM_ARG;
				return false;
			}
			stack->ip += sizeof(Crispr_VmLd);
			Crispr_Word ptr;
			Crispr_Word off;
			Crispr_Word mult;
	}
	return true;
}

static bool Crispr_vmGetRef(Crispr_VmRef* restrict ref, Crispr_VM* restrict vm, Crispr_Errno* restrict err) {
	Crispr_Stack* stack = vm->stack;
	switch (*(Crispr_VmLd*)&vm->code[stack->ip]) {
		case CRISPR_VMLD_LIT:
			if (err)
				*err = CRISPR_ERR_VM_OP;
			return false;
		case CRISPR_VMLD_OFF:
	}
	return true;
}

DLL_PUBLIC bool Crispr_vmRun(Crispr_VM* restrict vm, Crispr_Size exec, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	Crispr_Stack* stack = vm->stack;
	if (!stack) {
		if (err)
			*err = CRISPR_ERR_NULL;
		return false;
	}
	while (true) {
		if (*(Crispr_VmOp*)&vm->code[stack->ip] == CRISPR_VMOP_NOOP) {
			stack->ip += sizeof(Crispr_VmOp);
			continue;
		}
		if (*(Crispr_VmOp*)&vm->code[stack->ip] == CRISPR_VMOP_HALT)
			break;
		if (exec > 0) {
			if (--exec == 0) {
				if (err)
					*err = CRISPR_ERR_LIMIT;
				return false;
			}
		}
	}
	return true;
}

DLL_RESTORE
