#include "headers/Class.h"
#include "headers/Errno.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_protoClass(Crispr_ProtoClass* restrict dst, const char* restrict name, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	dst->main.proto = true;
	dst->name = name;
	return true;
}

DLL_RESTORE
