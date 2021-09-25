#include "headers/VM.h"
#include "headers/Memory.h"
#include "headers/Error.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_vmStackInit(Crispr_VmStack* restrict stack, Crispr_VM* vm, Crispr_Size init, Crispr_Size len, Crispr_Errno* err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	if (len == 0)
		len = 16;
	stack->origin = Crispr_malloc(len);
	if (!stack->origin) {
		*err = CRISPR_ERR_NOMEM;
		return false;
	}
	if (mtx_lock(&vm->lock) != thrd_success) {
		Crispr_free(stack->origin);
		stack->origin = Crispr_nullRef(char);
		if (err)
			*err = CRISPR_ERR_SYS;
		return false;
	}
	stack->len = len;
	stack->ip = init;
	mtx_unlock(&vm->lock);
	return true;
}

DLL_PUBLIC bool Crispr_vmInit(Crispr_VM* restrict vm, const Crispr_VmNameSpace* ns, Crispr_Errno* err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	if (mtx_init(&vm->lock, mtx_plain) != thrd_success) {
		if (err)
			*err = CRISPR_ERR_SYS;
		vm->base = Crispr_nullRef(Crispr_VmNameSpace);
		return false;
	}
	vm->stack = Crispr_nullRef(Crispr_VmStack);
	vm->base = ns;
	return true;
}

typedef struct {
	short flags; //holds flag type
	union {
		void* ptr;
		Crispr_Size off;
	};
} Crispr_VmRef;

static bool crispr_vm_getWord(Crispr_VmWord* restrict wrd, Crispr_VmStack* restrict stack, bool offset,
		Crispr_Errno* restrict err);

static bool crispr_vm_offset(Crispr_VmWord* restrict res, Crispr_VmStack* restrict stack, Crispr_Errno* restrict err) {
	switch (*(Crispr_VmOf*)&stack->cur_seg[stack->ip]) {
		case CRISPR_VMOF_MEM:
			stack->ip += sizeof(Crispr_VmOf);
			if (!crispr_vm_getWord(res, stack, false, err))
				return false;
			break;
		case CRISPR_VMOF_COD:
			stack->ip += sizeof(Crispr_VmOf);
			res->cptr = stack->cur_seg->code;
			break;
		case CRISPR_VMOF_CUR:
			res->cptr = &stack->cur_seg->code[stack->ip];
			stack->ip += sizeof(Crispr_VmOf);
			break;
		case CRISPR_VMOF_STK:
			res->ptr = &stack->origin[stack->ip];
			break;
		default:
			if (err)
				*err = CRISPR_ERR_VM_ARG;
			return false;
	}
	Crispr_VmWord off;
	if (!crispr_vm_getWord(&off, stack, false, err))
		return false;
	Crispr_VmWord mult;
	if (!crispr_vm_getWord(&mult, stack, false, err))
		return false;
	res->ptr += (off.off * mult.off);
	return true;
}

static bool crispr_vm_getWord(Crispr_VmWord* restrict wrd, Crispr_VmStack* restrict stack, bool offset,
		Crispr_Errno* restrict err) {
	switch (*(Crispr_VmLd*)&stack->cur_seg->code[stack->ip]) {
		case CRISPR_VMLD_LIT:
			stack->ip += sizeof(Crispr_VmLd);
			*wrd = *(Crispr_VmWord*)(&stack->cur_seg->code[stack->ip]);
			stack->ip += sizeof(Crispr_VmWord);
			return true;
		case CRISPR_VMLD_OFF:
			if (!offset) {
				if (err)
					*err = CRISPR_ERR_VM_ARG;
				return false;
			}
			stack->ip += sizeof(Crispr_VmLd);
	}
	return true;
}

static bool Crispr_vmGetRef(Crispr_VmRef* restrict ref, Crispr_VmStack* restrict stack, Crispr_Errno* restrict err) {
	switch (*(Crispr_VmLd*)&stack->cur_seg->code[stack->ip]) {
		case CRISPR_VMLD_LIT:
			if (err)
				*err = CRISPR_ERR_VM_OP;
			return false;
	}
	return true;
}

DLL_PUBLIC bool Crispr_vmRun(Crispr_VmStack* restrict stack, Crispr_Size exec, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	while (true) {
		if (*(Crispr_VmOp*)&stack->cur_seg->code[stack->ip] == CRISPR_VMOP_NOOP) {
			stack->ip += sizeof(Crispr_VmOp);
			continue;
		}
		if (*(Crispr_VmOp*)&stack->cur_seg->code[stack->ip] == CRISPR_VMOP_HALT)
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
