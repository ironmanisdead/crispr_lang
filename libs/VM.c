#include "headers/VM.h"
#include "headers/Memory.h"
#include "headers/Error.h"
#include "headers/String.h"

DLL_HIDE

#define crispr_realign(type, stack) stack->ip = Crispr_nextBase(type, stack->cur_seg->code, stack->ip)

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
	Crispr_VmStack** stk = &vm->stack;
	while (*stk != Crispr_nullRef(Crispr_VmStack))
		stk = &(*stk)->next;
	*stk = stack;
	stack->from = stk;
	stack->next = Crispr_nullRef(Crispr_VmStack);
	stack->len = len;
	stack->ip = init;
	stack->frame = 0;
	stack->end = 0;
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

static bool crispr_vm_getWord(Crispr_VmWord* restrict wrd, Crispr_VmStack* restrict stack, Crispr_VmSz offset,
		Crispr_Errno* restrict err);

static bool crispr_vm_offset(Crispr_VmWord* restrict res, Crispr_VmStack* restrict stack, Crispr_Errno* restrict err) {
	crispr_realign(Crispr_VmOf, stack);
	switch (*(Crispr_VmOf*)&stack->cur_seg[stack->ip]) {
		case CRISPR_VMOF_MEM:
			stack->ip += sizeof(Crispr_VmOf);
			if (!crispr_vm_getWord(res, stack, CRISPR_VMSZ_NPTR, err))
				return false;
			break;
		case CRISPR_VMOF_COD:
			stack->ip += sizeof(Crispr_VmOf);
			res->cptr = stack->cur_seg->code;
			break;
		case CRISPR_VMOF_CUR:
			stack->ip += sizeof(Crispr_VmOf);
			res->cptr = &stack->cur_seg->code[stack->ip];
			break;
		case CRISPR_VMOF_STK:
			stack->ip += sizeof(Crispr_VmOf);
			res->ptr = stack->origin;
			break;
		case CRISPR_VMOF_FRM:
			stack->ip += sizeof(Crispr_VmOf);
			res->ptr = &stack->origin[stack->frame];
			break;
		case CRISPR_VMOF_SYM:
			stack->ip += sizeof(Crispr_VmOf);
			Crispr_Off off = Crispr_strIter(&stack->cur_seg->code[stack->ip], &Crispr_symVm, err);
			if (off < 0) {
				if (err)
					*err = CRISPR_ERR_VM_SYMVAL;
				return false;
			}
			break;
			/*
		default:
			if (err)
				*err = CRISPR_ERR_VM_ARG;
			return false;
			*/
	}
	Crispr_VmWord off;
	if (!crispr_vm_getWord(&off, stack, CRISPR_VMSZ_NSIZ, err))
		return false;
	Crispr_VmWord mult;
	if (!crispr_vm_getWord(&mult, stack, CRISPR_VMSZ_NSIZ, err))
		return false;
	res->ptr += (off.off * mult.off);
	return true;
}

static bool crispr_vm_setWord(Crispr_VmWord* restrict dst, const Crispr_VmWord* restrict src, Crispr_VmSz size, Crispr_Errno* err) {
	switch (size) {
		case CRISPR_VMSZ_NPTR:
			dst->ptr = src->ptr;
			break;
		case CRISPR_VMSZ_NSIZ:
			dst->size = src->size;
			break;
		case CRISPR_VMSZ_WORD:
			*dst = *src;
			break;
		case CRISPR_VMSZ_SIZE:
			dst->size = src->size;
			break;
		case CRISPR_VMSZ_PNTR:
			dst->ptr = src->ptr;
			break;
		case CRISPR_VMSZ_BYTE:
			dst->ubyte = src->ubyte;
			break;
		case CRISPR_VMSZ_SHRT:
			dst->ushrt = src->ushrt;
			break;
		case CRISPR_VMSZ_INTG:
			dst->uint = src->uint;
			break;
		case CRISPR_VMSZ_LONG:
			dst->ulong = src->ulong;
			break;
		case CRISPR_VMSZ_FLOT:
			dst->flt = src->flt;
			break;
		case CRISPR_VMSZ_DUBL:
			dst->dbl = src->dbl;
			break;
		default:
			if (err)
				*err = CRISPR_ERR_VM_ARG;
			return false;
	}
	return true;
}

static bool crispr_vm_getWord(Crispr_VmWord* restrict wrd, Crispr_VmStack* restrict stack, Crispr_VmSz size,
		Crispr_Errno* restrict err) {
	crispr_realign(Crispr_VmLd, stack);
	switch (*(Crispr_VmLd*)&stack->cur_seg->code[stack->ip]) {
		case CRISPR_VMLD_LIT:
			stack->ip += sizeof(Crispr_VmLd);
			crispr_realign(Crispr_VmWord, stack);
			*wrd = *(Crispr_VmWord*)(&stack->cur_seg->code[stack->ip]);
			stack->ip += sizeof(Crispr_VmWord);
			return true;
		case CRISPR_VMLD_OFF:
			if ((size == CRISPR_VMSZ_NPTR) || (size == CRISPR_VMSZ_NSIZ)) {
				if (err)
					*err = CRISPR_ERR_VM_ARG;
				return false;
			}
			stack->ip += sizeof(Crispr_VmLd);
			Crispr_VmWord wrd;
			if (!crispr_vm_offset(&wrd, stack, err))
				return false;
	}
	return true;
}

static bool crispr_vm_getRef(Crispr_VmRef* restrict ref, Crispr_VmStack* restrict stack, Crispr_Errno* restrict err) {
	crispr_realign(Crispr_VmLd, stack);
	switch (*(Crispr_VmLd*)&stack->cur_seg->code[stack->ip]) {
		case CRISPR_VMLD_LIT:
			if (err)
				*err = CRISPR_ERR_VM_OP;
			return false;
		case CRISPR_VMLD_FLG:
			stack->ip += sizeof(Crispr_VmLd);
			crispr_realign(short, stack);
			ref->flags = *(short*)&stack->cur_seg->code[stack->ip];
			if (!ref->flags) {
				if (err)
					*err = CRISPR_ERR_VM_OP;
				return false;
			}
			break;
	}
	return true;
}

DLL_PUBLIC Crispr_LoopStat Crispr_vmRun(Crispr_VmStack* restrict stack, Crispr_Size exec, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	while (true) {
		crispr_realign(Crispr_VmOp, stack);
		Crispr_VmRef ref;
		Crispr_VmWord word;
		switch (*(Crispr_VmOp*)&stack->cur_seg->code[stack->ip]) {
			case CRISPR_VMOP_NOOP:
				stack->ip += sizeof(Crispr_VmOp);
				break;
			case CRISPR_VMOP_STOP:
				stack->ip += sizeof(Crispr_VmOp);
				return CRISPR_LOOP_CONT;
			case CRISPR_VMOP_HALT:
				return CRISPR_LOOP_DONE;
			case CRISPR_VMOP_MOVE:
				stack->ip += sizeof(Crispr_VmOp);
				crispr_realign(Crispr_VmSz, stack);
				Crispr_VmSz size = *(Crispr_VmSz*)&stack->cur_seg->code[stack->ip];
				stack->ip += sizeof(Crispr_VmSz);
				if (!crispr_vm_getRef(&ref, stack, err))
					return CRISPR_LOOP_FAIL;
				if (!crispr_vm_getWord(&word, stack, size, err))
					return CRISPR_LOOP_FAIL;
				break;
		}
		if (exec > 0) {
			if (--exec == 0) {
				if (err)
					*err = CRISPR_ERR_LIMIT;
				return CRISPR_LOOP_FAIL;
			}
		}
	}
}

DLL_RESTORE
