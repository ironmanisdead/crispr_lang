#include "headers/VM.h"
#include "headers/Memory.h"
#include "headers/Error.h"
#include "headers/String.h"
#include <string.h>

DLL_HIDE

#define crispr_realign(type, stack) stack->rp = Crispr_nextBase(type, stack->segc->code, stack->rp)

DLL_PUBLIC bool Crispr_vmStackInit(Crispr_VmStk* restrict stack,
		Crispr_VM* vm, Crispr_VmCdSeg* restrict ns, Crispr_Size len,
		Crispr_Size start, Crispr_Errno* err) {
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
	Crispr_VmStk** stk = &vm->stack;
	while (*stk != Crispr_nullRef(Crispr_VmStk))
		stk = &(*stk)->next;
	*stk = stack;
	stack->from = stk;
	stack->next = Crispr_nullRef(Crispr_VmStk);
	stack->len = len;
	stack->sp = start;
	stack->rp = start;
	stack->segc = ns;
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
	vm->stack = Crispr_nullRef(Crispr_VmStk);
	vm->base = ns;
	return true;
}

typedef struct {
	Crispr_VmCdSeg* segcent;
	const char* code;
} Crispr_VmSym;

typedef struct {
	Crispr_LoopStat stat;
	const char* addr;
} Crispr_VmPtr;

static Crispr_VmPtr crispr_vm_symFrom(const char* restrict header, const char* restrict query,
		Crispr_Errno* restrict err) {
	Crispr_Off qsize = strlen(query);
	if (qsize < 0)
		return (Crispr_VmPtr){ CRISPR_LOOP_FAIL, Crispr_nullRef(char) };
	if (!header)
		return (Crispr_VmPtr){ CRISPR_LOOP_CONT, Crispr_nullRef(char) };
	while (true) {
		if (*header == '\0')
			return (Crispr_VmPtr){ CRISPR_LOOP_CONT, Crispr_nullRef(char) };
		Crispr_Off off = Crispr_strIter(header, &Crispr_symVm, err);
		if (off < 0) {
			return (Crispr_VmPtr){ CRISPR_LOOP_FAIL, Crispr_nullRef(char) };
		}
		bool comp = (memcmp(header, query, off) == 0);
		header += off + 1;
		header = (const char*)Crispr_nextPtr(const char*, header);
		if (comp)
			return (Crispr_VmPtr){ CRISPR_LOOP_DONE, *(const char**)(header) };
		header += sizeof(const char*);
	}
}

static Crispr_VmSym crispr_vm_symFind(Crispr_VmCdSeg* restrict code, const char* restrict query,
		Crispr_Errno* restrict err);

static bool crispr_vm_getWord(Crispr_VmWrd* restrict wrd, Crispr_VmStk* restrict stack, Crispr_VmTy type,
		Crispr_Errno* restrict err);

static bool crispr_vm_offset(Crispr_VmRef* restrict res, Crispr_VmStk* restrict stack, Crispr_Errno* restrict err) {
	crispr_realign(Crispr_VmOf, stack);
	Crispr_VmWrd wrd;
	res->flags = 0;
	switch (*(Crispr_VmOf*)&stack->segc[stack->rp]) {
		case CRISPR_VMOF_MEM:
			stack->rp += sizeof(Crispr_VmOf);
			if (!crispr_vm_getWord(&wrd, stack, CRISPR_VMSZ_PNTR, err))
				return false;
			res->ptr.addr = (char*)wrd.cptr;
			break;
		case CRISPR_VMOF_COD:
			stack->rp += sizeof(Crispr_VmOf);
			res->ptr.addr = (char*)stack->segc->code;
			break;
		case CRISPR_VMOF_CUR:
			stack->rp += sizeof(Crispr_VmOf);
			res->ptr.addr = (char*)&stack->segc->code[stack->sp];
			break;
		case CRISPR_VMOF_STK:
			stack->rp += sizeof(Crispr_VmOf);
			res->ptr.addr = (char*)stack->origin;
			break;
		case CRISPR_VMOF_FRM:
			stack->rp += sizeof(Crispr_VmOf);
			res->ptr.addr = (char*)&stack->origin[stack->frame];
			break;
		case CRISPR_VMOF_SYM:
			stack->rp += sizeof(Crispr_VmOf);
			Crispr_Off calc = Crispr_strIter(&stack->segc->code[stack->rp], &Crispr_symVm, err);
			if (calc < 0) {
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
	Crispr_VmWrd off;
	if (!crispr_vm_getWord(&off, stack, CRISPR_VMSZ_SIZE, err))
		return false;
	Crispr_VmWrd mult;
	if (!crispr_vm_getWord(&mult, stack, CRISPR_VMSZ_SIZE, err))
		return false;
	res->ptr.addr += (off.size * mult.size);
	return true;
}

static bool crispr_vm_setWord(Crispr_VmWrd* restrict dst, const Crispr_VmWrd* restrict src, Crispr_VmTy type, Crispr_Errno* err) {
	if (Crispr_vmSz(type) == CRISPR_VMSZ_PNTR) {
		dst->ptr = src->ptr;
		return true;
	}
	switch (Crispr_vmSz(type)) {
		case CRISPR_VMSZ_WORD:
			*dst = *src;
			break;
		case CRISPR_VMSZ_SIZE:
			dst->size = src->size;
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

static bool crispr_vm_getWord(Crispr_VmWrd* restrict wrd, Crispr_VmStk* restrict stack, Crispr_VmTy type,
		Crispr_Errno* restrict err) {
	crispr_realign(Crispr_VmLd, stack);
	switch (*(Crispr_VmLd*)&stack->segc->code[stack->rp]) {
		case CRISPR_VMLD_LIT:
			stack->rp += sizeof(Crispr_VmLd);
			crispr_realign(Crispr_VmWrd, stack);
			*wrd = *(Crispr_VmWrd*)(&stack->segc->code[stack->rp]);
			stack->rp += sizeof(Crispr_VmWrd);
			return true;
		case CRISPR_VMLD_OFF:
			if (type & CRISPR_VMTY_DREF) {
				if (err)
					*err = CRISPR_ERR_VM_ARG;
				return false;
			}
			stack->rp += sizeof(Crispr_VmLd);
			Crispr_VmRef wrd;
			if (!crispr_vm_offset(&wrd, stack, err))
				return false;
	}
	return true;
}

static bool crispr_vm_getRef(Crispr_VmRef* restrict ref, Crispr_VmStk* restrict stack, Crispr_Errno* restrict err) {
	crispr_realign(Crispr_VmLd, stack);
	switch (*(Crispr_VmLd*)&stack->segc->code[stack->rp]) {
		case CRISPR_VMLD_LIT:
			if (err)
				*err = CRISPR_ERR_VM_OP;
			return false;
	}
	return true;
}

DLL_PUBLIC Crispr_LoopStat Crispr_vmRun(Crispr_VmStk* restrict stack, Crispr_Size exec, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	stack->rp = stack->sp;
	while (true) {
		crispr_realign(Crispr_VmOp, stack);
		stack->sp = stack->rp;
		Crispr_VmRef ref;
		Crispr_VmWrd word;
		switch (*(Crispr_VmOp*)&stack->segc->code[stack->rp]) {
			case CRISPR_VMOP_NOOP:
				stack->rp += sizeof(Crispr_VmOp);
				break;
			case CRISPR_VMOP_STOP:
				stack->rp += sizeof(Crispr_VmOp);
				return CRISPR_LOOP_CONT;
			case CRISPR_VMOP_HALT:
				return CRISPR_LOOP_DONE;
			case CRISPR_VMOP_MOV:
				stack->rp += sizeof(Crispr_VmOp);
				crispr_realign(Crispr_VmSz, stack);
				Crispr_VmTy type = *(Crispr_VmTy*)&stack->segc->code[stack->rp];
				stack->rp += sizeof(Crispr_VmTy);
				if (!crispr_vm_getRef(&ref, stack, err))
					return CRISPR_LOOP_FAIL;
				if (!crispr_vm_getWord(&word, stack, type, err))
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
