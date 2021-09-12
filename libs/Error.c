#include "headers/Error.h"

DLL_HIDE

#define CRISPR_MAKEERRTYPE(name, info) DLL_PUBLIC const Crispr_Error _Crispr_cn_ERR##name = { #name "\0" info, CRISPR_NULL }
#define CRISPR_MAKEERRBASE(name, ...) static const Crispr_Errno crispr_errbase_##name[] = { __VA_ARGS__, CRISPR_NULL }
#define CRISPR_MAKEERRFULL(name, info, base) DLL_PUBLIC const Crispr_Error _Crispr_cn_ERR##name = { #name "\0" info, crispr_errbase_##base }

CRISPR_MAKEERRTYPE(SYS, "System error.");
CRISPR_MAKEERRBASE(system, CRISPR_ERRSYS);
CRISPR_MAKEERRFULL(RESOURCE, "System resource error.", system);
CRISPR_MAKEERRBASE(resource, CRISPR_ERRRESOURCE);
CRISPR_MAKEERRTYPE(PTR, "Pointer error");

CRISPR_MAKEERRTYPE(INVAL, "Invalid value error.");
CRISPR_MAKEERRBASE(invalid, CRISPR_ERRINVAL);
CRISPR_MAKEERRFULL(SYMBOL, "Error in converting string to valid symbol.", invalid);
CRISPR_MAKEERRBASE(invalid_pointer, CRISPR_ERRINVAL, CRISPR_ERRBADPTR);
CRISPR_MAKEERRFULL(BADPTR, "Operation was given an invalid pointer, when it needed a valid address.", invalid_pointer);
CRISPR_MAKEERRBASE(badptr, CRISPR_ERRBADPTR);
CRISPR_MAKEERRFULL(NULL, "Null pointer given for operation, when operation needed a valid address.", badptr);
CRISPR_MAKEERRFULL(FAULT, "Segmentation fault in userspace", badptr);

CRISPR_MAKEERRFULL(NOMEM, "Not enough memory for operation.", resource);
CRISPR_MAKEERRTYPE(OBJECT, "Object encountered error.");
CRISPR_MAKEERRBASE(object, CRISPR_ERROBJECT);
CRISPR_MAKEERRFULL(INIT, "Operation could not initialize object completely.", object);
CRISPR_MAKEERRFULL(INDEX, "Request out of range for object.", object);

CRISPR_MAKEERRTYPE(MATH, "Operation encountered mathematical error.");
CRISPR_MAKEERRBASE(math, CRISPR_ERRMATH);
CRISPR_MAKEERRFULL(DOMAIN, "Parameter outside of mathematical domain.", math);
CRISPR_MAKEERRFULL(RANGE, "Overflow encountered in result of operation.", math);

CRISPR_MAKEERRFULL(TYPE, "Operation encountered type error.", object);
CRISPR_MAKEERRBASE(type, CRISPR_ERRTYPE);
CRISPR_MAKEERRFULL(CONV, "Operation could not convert type into another.", type);
CRISPR_MAKEERRFULL(ATTR, "Type had attributes that were missing or inappropriate for operation.", type);
CRISPR_MAKEERRFULL(EMPTY, "Object was empty, when a non-empty object was expected.", object);
CRISPR_MAKEERRFULL(NOTEMPTY, "Object had things in it, when an empty object was expected.", object);

CRISPR_MAKEERRTYPE(INTR, "Operation was interrupted by a change.");
CRISPR_MAKEERRBASE(change, CRISPR_ERRINTR);
CRISPR_MAKEERRFULL(TIMEDOUT, "Operation was timed out.", change);
CRISPR_MAKEERRFULL(STALE, "Object is currently unusable for operation.", object);
CRISPR_MAKEERRBASE(dead_pre, CRISPR_ERRINTR, CRISPR_ERRSTALE);
CRISPR_MAKEERRFULL(DEAD, "Object became stale during operation.", dead_pre);

CRISPR_MAKEERRFULL(ACCESS, "Operation cannot access object", object);
CRISPR_MAKEERRBASE(access, CRISPR_ERRACCESS);
CRISPR_MAKEERRFULL(PERM, "Operation does not have adequate permissions to use object.", access);
CRISPR_MAKEERRTYPE(AGAIN, "Operation could not be preformed at this time.");

DLL_PUBLIC const char* Crispr_errName(Crispr_Errno err) {
	if (err == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	return err->errdata;
}

DLL_PUBLIC const char* Crispr_errDesc(Crispr_Errno err) {
	if (err == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	const char* restrict data = err->errdata;
	while (*data != '\0')
		data++;
	return data + 1;
}

DLL_PUBLIC const Crispr_Errno* Crispr_errBases(Crispr_Errno err) {
	if (err == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	return err->bases;
}

DLL_PUBLIC bool Crispr_errIsA(Crispr_Errno err, Crispr_Errno cmp) {
	if (err == cmp)
		return true;
	if ((err && true) != (cmp && true))
		return false;
	if (err->bases == CRISPR_NULL)
		return false;
	for (const Crispr_Errno* restrict item = err->bases; *item != CRISPR_NULL; item++) {
		if ((*item == cmp) || Crispr_errIsA(err, *item))
			return true;
	}
	return false;
}

DLL_RESTORE
