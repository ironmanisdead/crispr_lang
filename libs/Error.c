#include "headers/Error.h"

DLL_HIDE

#define CRISPR_MAKEERRTYPE(name, info) const Crispr_Error _Crispr_cn_ERR##name = { #name "\0" info, CRISPR_NULL }
#define CRISPR_MAKEERRBASE(name, ...) static const Crispr_Errno crispr_errbase_##name[] = { __VA_ARGS__ }
#define CRISPR_MAKEERRFULL(name, info, base) const Crispr_Error _Crispr_cn_ERR##name = { #name "\0" info, crispr_errbase_##base }

CRISPR_MAKEERRTYPE(SYS, "Unknown system error.");
CRISPR_MAKEERRTYPE(INVAL, "Invalid value error.");
CRISPR_MAKEERRBASE(invalid, CRISPR_ERRINVAL);
CRISPR_MAKEERRFULL(SYMBOL, "Error in converting string to valid symbol.", invalid);
CRISPR_MAKEERRTYPE(NULL, "Null pointer error.");
CRISPR_MAKEERRTYPE(NOMEM, "Not enough memory for operation");

DLL_PUBLIC const char* Crispr_errname(Crispr_Errno stat) {
	if (stat == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	return stat->errdata;
}

DLL_PUBLIC const char* Crispr_errdesc(Crispr_Errno stat) {
	if (stat == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	const char* restrict data = stat->errdata;
	while (*data != '\0')
		data++;
	return data + 1;
}

DLL_RESTORE
