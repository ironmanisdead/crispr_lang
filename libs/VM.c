#include "headers/VM.h"
#include "headers/Memory.h"
#include "headers/Error.h"

DLL_HIDE

static Crispr_Size _crispr_realign(Crispr_Size off, Crispr_Size align) {
	Crispr_Size calc = (off / align) * align;
	if (calc < off) {
		calc += align;
	}
	return calc;
}

#define crispr_realign(type, off) off = _crispr_realign(off, _Alignof(type))

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
	crispr_realign(Crispr_VmOf, stack->ip);
	switch (*(Crispr_VmOf*)&stack->cur_seg[stack->ip]) {
		case CRISPR_VMOF_MEM:
			stack->ip += sizeof(Crispr_VmOf);
			if (!crispr_vm_getWord(res, stack, CRISPR_VMSZ_NONE, err))
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
	if (!crispr_vm_getWord(&off, stack, CRISPR_VMSZ_NONE, err))
		return false;
	Crispr_VmWord mult;
	if (!crispr_vm_getWord(&mult, stack, CRISPR_VMSZ_NONE, err))
		return false;
	res->ptr += (off.off * mult.off);
	return true;
}

static bool crispr_vm_setWord(Crispr_VmWord* restrict dst, const Crispr_VmWord* restrict src, Crispr_VmSz size, Crispr_Errno* err) {
	switch (size) {
		case CRISPR_VMSZ_NONE:
			dst->ptr = src->ptr;
			break;
		case CRISPR_VMSZ_WORD:
			*dst = *src;
			break;
		case CRISPR_VMSZ_SIZE:
			dst->usize = src->usize;
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
	}
}

static bool crispr_vm_getWord(Crispr_VmWord* restrict wrd, Crispr_VmStack* restrict stack, Crispr_VmSz size,
		Crispr_Errno* restrict err) {
	crispr_realign(Crispr_VmLd, stack->ip);
	switch (*(Crispr_VmLd*)&stack->cur_seg->code[stack->ip]) {
		case CRISPR_VMLD_LIT:
			stack->ip += sizeof(Crispr_VmLd);
			crispr_realign(Crispr_VmWord, stack->ip);
			*wrd = *(Crispr_VmWord*)(&stack->cur_seg->code[stack->ip]);
			stack->ip += sizeof(Crispr_VmWord);
			return true;
		case CRISPR_VMLD_OFF:
			if (size == CRISPR_VMSZ_NONE) {
				if (err)
					*err = CRISPR_ERR_VM_ARG;
				return false;
			}
			stack->ip += sizeof(Crispr_VmLd);
	}
	return true;
}

static bool crispr_vm_getRef(Crispr_VmRef* restrict ref, Crispr_VmStack* restrict stack, Crispr_Errno* restrict err) {
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
		crispr_realign(Crispr_VmOp, stack->ip);
		Crispr_VmRef ref;
		Crispr_VmWord word;
		switch (*(Crispr_VmOp*)&stack->cur_seg->code[stack->ip]) {
			case CRISPR_VMOP_NOOP:
				stack->ip += sizeof(Crispr_VmOp);
				break;
			case CRISPR_VMOP_STOP:
				stack->ip += sizeof(Crispr_VmOp);
				/* fall through */
			case CRISPR_VMOP_HALT:
				return true;
			case CRISPR_VMOP_MOVE:
				stack->ip += sizeof(Crispr_VmOp);
				crispr_realign(Crispr_VmSz, stack->ip);
				Crispr_VmSz size = *(Crispr_VmSz*)&stack->cur_seg->code[stack->ip];
				stack->ip += sizeof(Crispr_VmSz);
				if (!crispr_vm_getRef(&ref, stack, err))
					return false;
				if (!crispr_vm_getWord(&word, stack, size, err))
					return false;
				break;
		}
		if (exec > 0) {
			if (--exec == 0) {
				if (err)
					*err = CRISPR_ERR_LIMIT;
				return false;
			}
		}
	}
}

DLL_RESTORE
