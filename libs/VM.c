#include "headers/VM.h"
#include "headers/Memory.h"
#include "headers/Error.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_stackInit(Crispr_Stack* restrict stack, const char* code,
		Crispr_Size len, Crispr_Errno* err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	if (len == 0)
		len = 16;
	stack->data = Crispr_malloc(len);
	if (!stack->data) {
		*err = CRISPR_ERRNOMEM;
		return false;
	}
	stack->len = len;
	stack->code = code;
	return true;
}

DLL_PUBLIC bool Crispr_vmRun(Crispr_VM* restrict vm, Crispr_Size exec, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	Crispr_Stack* stack = vm->stack;
	if (!stack) {
		if (err)
			*err = CRISPR_ERRNULL;
		return false;
	}
	if (!stack->code) {
		if (err)
			*err = CRISPR_ERRNULL;
		return false;
	}
	while (true) {
		if (*(Crispr_VmOp*)stack->code == CRISPR_VMOP_NOOP) {
			stack->code += sizeof(Crispr_VmOp);
			continue;
		}
		if (*(Crispr_VmOp*)stack->code == CRISPR_VMOP_HALT)
			break;
		if (exec > 0) {
			if (--exec == 0) {
				if (err)
					*err = CRISPR_ERRLIMIT;
				return false;
			}
		}
	}
	return true;
}

DLL_RESTORE
