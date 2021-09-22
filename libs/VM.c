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

DLL_RESTORE
