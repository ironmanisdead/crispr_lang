#include "headers/Class.h"
#include "headers/Error.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_protoClass(Crispr_ProtoClass* restrict dst, const char* restrict name, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	dst->main.proto = true;
	dst->name = name;
	return true;
}

DLL_RESTORE
