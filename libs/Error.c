#include "headers/Error.h"
#include "headers/.part/Error.h"

DLL_HIDE

DLL_PUBLIC const char* Crispr_errName(Crispr_Errno err) {
	if (err == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	if (err->data.fixed)
		return err->data.errconst;
	return err->data.erralloc;
}

DLL_PUBLIC const char* Crispr_errDesc(Crispr_Errno err) {
	if (err == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	const char* restrict data;
	if (err->data.fixed)
		data = err->data.errconst;
	else
		data = err->data.erralloc;
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
		if ((*item == cmp) || Crispr_errIsA(*item, cmp))
			return true;
	}
	return false;
}

_Crispr_MakeErrType(SYS, "System error.");
_Crispr_MakeErrBase(system, CRISPR_ERRSYS);
_Crispr_MakeErrFull(RESOURCE, "System resource error.", system);
_Crispr_MakeErrBase(resource, CRISPR_ERRRESOURCE);
_Crispr_MakeErrType(PTR, "Pointer error");

_Crispr_MakeErrType(INVAL, "Invalid value error.");
_Crispr_MakeErrBase(invalid, CRISPR_ERRINVAL);
_Crispr_MakeErrFull(SYMBOL, "Error in converting string to valid symbol.", invalid);
_Crispr_MakeErrBase(invalid_pointer, CRISPR_ERRINVAL, CRISPR_ERRBADPTR);
_Crispr_MakeErrFull(BADPTR, "Operation was given an invalid pointer, when it needed a valid address.", invalid_pointer);
_Crispr_MakeErrBase(badptr, CRISPR_ERRBADPTR);
_Crispr_MakeErrFull(NULL, "Null pointer given for operation, when operation needed a valid address.", badptr);
_Crispr_MakeErrFull(FAULT, "Segmentation fault in userspace", badptr);

_Crispr_MakeErrFull(NOMEM, "Not enough memory for operation.", resource);
_Crispr_MakeErrType(OBJECT, "Object encountered error.");
_Crispr_MakeErrBase(object, CRISPR_ERROBJECT);
_Crispr_MakeErrFull(INIT, "Operation could not initialize object completely.", object);
_Crispr_MakeErrFull(INDEX, "Request out of range for object.", object);

_Crispr_MakeErrType(MATH, "Operation encountered mathematical error.");
_Crispr_MakeErrBase(math, CRISPR_ERRMATH);
_Crispr_MakeErrFull(DOMAIN, "Parameter outside of mathematical domain.", math);
_Crispr_MakeErrFull(RANGE, "Overflow encountered in result of operation.", math);

_Crispr_MakeErrFull(TYPE, "Operation encountered type error.", object);
_Crispr_MakeErrBase(type, CRISPR_ERRTYPE);
_Crispr_MakeErrFull(CONV, "Operation could not convert type into another.", type);
_Crispr_MakeErrFull(ATTR, "Type had attributes that were missing or inappropriate for operation.", type);
_Crispr_MakeErrFull(EMPTY, "Object was empty, when a non-empty object was expected.", object);
_Crispr_MakeErrFull(NOTEMPTY, "Object had things in it, when an empty object was expected.", object);

_Crispr_MakeErrType(CHNG, "Operation was interrupted by a change.");
_Crispr_MakeErrBase(change, CRISPR_ERRCHNG);
_Crispr_MakeErrBase(signal_pre, CRISPR_ERRCHNG, CRISPR_ERRSYS);
_Crispr_MakeErrFull(SIG, "Operation was interrupted by a system signal.", signal_pre);
_Crispr_MakeErrFull(TIMEDOUT, "Operation was timed out.", change);
_Crispr_MakeErrFull(STALE, "Object is currently unusable for operation.", object);
_Crispr_MakeErrBase(dead_pre, CRISPR_ERRCHNG, CRISPR_ERRSTALE, CRISPR_ERROBJECT);
_Crispr_MakeErrFull(DEAD, "Object became stale during operation.", dead_pre);
_Crispr_MakeErrFull(AGAIN, "Operation could not be preformed at this time.", change);

_Crispr_MakeErrFull(ACCESS, "Operation cannot access object", object);
_Crispr_MakeErrBase(access, CRISPR_ERRACCESS);
_Crispr_MakeErrFull(PERM, "Operation does not have adequate permissions to use object.", access);

DLL_RESTORE
