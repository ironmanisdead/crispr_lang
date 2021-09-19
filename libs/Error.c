#include "headers/Error.h"
#include "headers/.part/Error.h"
#include "headers/Symbol.h"

DLL_HIDE

DLL_PUBLIC const char* Crispr_errName(Crispr_Errno err) {
	if (err == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	if (err->data.alloc)
		return err->data.erralloc;
	return err->data.errconst;
}

DLL_PUBLIC const char* Crispr_errDesc(Crispr_Errno err) {
	if (err == CRISPR_ERRNOERR)
		return CRISPR_NULL;
	const char* restrict data;
	if (err->data.alloc)
		data = err->data.erralloc;
	else
		data = err->data.errconst;
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

DLL_PUBLIC NONNULL(1, 2) char* _Crispr_errSymMake(const char* restrict name, const char* restrict desc, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	Crispr_Size len;
	if (!Crispr_symLen(&len, name, err))
		return CRISPR_NULL;
	char* result = Crispr_malloc(len + Crispr_strLen(desc) + 2);
	if (!result) {
		*err = CRISPR_ERRNOMEM;
		return CRISPR_NULL;
	}
	Crispr_strCpy(result, name);
	Crispr_strCpy(&result[len + 1], desc);
	return result;
}

DLL_PUBLIC NONNULL(1) bool _Crispr_errSet(Crispr_Error* restrict res, struct _Crispr_ErrData data,
		const Crispr_Errno* restrict bases, Crispr_Errno* restrict err) {
	if (data.alloc) {
		if (data.erralloc == CRISPR_NULL)
			return false;
	} else if (err) {
		*err = CRISPR_ERRNOERR;
	}
	res->data = data;
	res->bases = bases;
	return true;
}

DLL_PUBLIC NONNULL(1) bool _Crispr_errDynFree(Crispr_Error* restrict target, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERRNOERR;
	if (!target->data.alloc) {
		*err = CRISPR_ERRATTR;
		return false;
	} else if (target->data.erralloc == CRISPR_NULL) {
		*err = CRISPR_ERRSTALE;
		return false;
	}
	Crispr_free(target->data.erralloc);
	target->data.erralloc = CRISPR_NULL;
	return true;
}

_Crispr_makeErrType(SYS, "System error.");
_Crispr_makeErrBase(system, CRISPR_ERRSYS);
_Crispr_makeErrFull(RESOURCE, "System resource error.", system);
_Crispr_makeErrBase(resource, CRISPR_ERRRESOURCE);
_Crispr_makeErrType(PTR, "Pointer error");

_Crispr_makeErrType(INVAL, "Invalid value error.");
_Crispr_makeErrBase(invalid, CRISPR_ERRINVAL);
_Crispr_makeErrFull(SYMBOL, "Error in converting string to valid symbol.", invalid);
_Crispr_makeErrBase(invalid_pointer, CRISPR_ERRINVAL, CRISPR_ERRBADPTR);
_Crispr_makeErrFull(BADPTR, "Operation was given an invalid pointer, when it needed a valid address.", invalid_pointer);
_Crispr_makeErrBase(badptr, CRISPR_ERRBADPTR);
_Crispr_makeErrFull(NULL, "Null pointer given for operation, when operation needed a valid address.", badptr);
_Crispr_makeErrFull(FAULT, "Segmentation fault in userspace", badptr);

_Crispr_makeErrFull(NOMEM, "Not enough memory for operation.", resource);
_Crispr_makeErrType(OBJECT, "Object encountered error.");
_Crispr_makeErrBase(object, CRISPR_ERROBJECT);
_Crispr_makeErrFull(INIT, "Operation could not initialize object completely.", object);
_Crispr_makeErrFull(INDEX, "Request out of range for object.", object);

_Crispr_makeErrType(MATH, "Operation encountered mathematical error.");
_Crispr_makeErrBase(math, CRISPR_ERRMATH);
_Crispr_makeErrFull(DOMAIN, "Parameter outside of mathematical domain.", math);
_Crispr_makeErrFull(RANGE, "Overflow encountered in result of operation.", math);

_Crispr_makeErrFull(TYPE, "Operation encountered type error.", object);
_Crispr_makeErrBase(type, CRISPR_ERRTYPE);
_Crispr_makeErrFull(CONV, "Operation could not convert type into another.", type);
_Crispr_makeErrFull(ATTR, "Type had attributes that were missing or inappropriate for operation.", type);
_Crispr_makeErrFull(EMPTY, "Object was empty, when a non-empty object was expected.", object);
_Crispr_makeErrFull(NOTEMPTY, "Object had things in it, when an empty object was expected.", object);

_Crispr_makeErrType(CHNG, "Operation was interrupted by a change.");
_Crispr_makeErrBase(change, CRISPR_ERRCHNG);
_Crispr_makeErrBase(signal_pre, CRISPR_ERRCHNG, CRISPR_ERRSYS);
_Crispr_makeErrFull(SIG, "Operation was interrupted by a system signal.", signal_pre);
_Crispr_makeErrFull(TIMEDOUT, "Operation was timed out.", change);
_Crispr_makeErrFull(STALE, "Object is currently unusable for operation.", object);
_Crispr_makeErrBase(dead_pre, CRISPR_ERRCHNG, CRISPR_ERRSTALE, CRISPR_ERROBJECT);
_Crispr_makeErrFull(DEAD, "Object became stale during operation.", dead_pre);
_Crispr_makeErrFull(AGAIN, "Operation could not be preformed at this time.", change);

_Crispr_makeErrFull(ACCESS, "Operation cannot access object", object);
_Crispr_makeErrBase(access, CRISPR_ERRACCESS);
_Crispr_makeErrFull(PERM, "Operation does not have adequate permissions to use object.", access);

DLL_RESTORE
