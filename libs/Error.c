#include "headers/Error.h"
#include "headers/.part/Error.h"
#include "headers/Symbol.h"

DLL_HIDE

DLL_PUBLIC const char* Crispr_errName(Crispr_Errno err) {
	if (err == CRISPR_ERR_NOERR)
		return Crispr_nullRef(char);
	if (err->data.alloc)
		return err->data.erralloc;
	return err->data.errconst;
}

DLL_PUBLIC const char* Crispr_errDesc(Crispr_Errno err) {
	if (err == CRISPR_ERR_NOERR)
		return Crispr_nullRef(char);
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
	if (err == CRISPR_ERR_NOERR)
		return Crispr_nullRef(Crispr_Errno);
	return err->bases;
}

DLL_PUBLIC bool Crispr_errIsA(Crispr_Errno err, Crispr_Errno cmp) {
	if (err == cmp)
		return true;
	if ((err && true) != (cmp && true))
		return false;
	if (err->bases == Crispr_nullRef(Crispr_Errno))
		return false;
	for (const Crispr_Errno* restrict item = err->bases; *item != Crispr_nullRef(Crispr_Error); item++) {
		if ((*item == cmp) || Crispr_errIsA(*item, cmp))
			return true;
	}
	return false;
}

DLL_PUBLIC NONNULL(1, 2) char* _Crispr_errSymMake(const char* restrict name, const char* restrict desc, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	Crispr_Size len;
	if (!Crispr_symLen(&len, name, err))
		return Crispr_nullRef(char);
	char* result = Crispr_malloc(len + Crispr_strLen(desc) + 2);
	if (!result) {
		*err = CRISPR_ERR_NOMEM;
		return Crispr_nullRef(char);
	}
	Crispr_strCpy(result, name);
	Crispr_strCpy(&result[len + 1], desc);
	return result;
}

DLL_PUBLIC NONNULL(1) bool _Crispr_errSet(Crispr_Error* restrict res, struct _Crispr_ErrData data,
		const Crispr_Errno* restrict bases, Crispr_Errno* restrict err) {
	if (data.alloc) {
		if (data.erralloc == Crispr_nullRef(char))
			return false;
	} else if (err) {
		*err = CRISPR_ERR_NOERR;
	}
	res->data = data;
	res->bases = bases;
	return true;
}

DLL_PUBLIC NONNULL(1) bool _Crispr_errDynFree(Crispr_Error* restrict target, Crispr_Errno* restrict err) {
	if (err)
		*err = CRISPR_ERR_NOERR;
	if (!target->data.alloc) {
		*err = CRISPR_ERR_ATTR;
		return false;
	} else if (target->data.erralloc == Crispr_nullRef(char)) {
		*err = CRISPR_ERR_STALE;
		return false;
	}
	Crispr_free(target->data.erralloc);
	target->data.erralloc = Crispr_nullRef(char);
	return true;
}

_Crispr_makeErrType(SYS, "System error.");
_Crispr_makeErrBase(system, CRISPR_ERR_SYS);
_Crispr_makeErrFull(RESOURCE, "System resource error.", system);
_Crispr_makeErrBase(resource, CRISPR_ERR_RESOURCE);
_Crispr_makeErrType(PTR, "Pointer error");

_Crispr_makeErrType(INVAL, "Invalid value error.");
_Crispr_makeErrBase(invalid, CRISPR_ERR_INVAL);
_Crispr_makeErrFull(SYMBOL, "Error in converting string to valid symbol.", invalid);
_Crispr_makeErrBase(invalid_pointer, CRISPR_ERR_INVAL, CRISPR_ERR_BADPTR);
_Crispr_makeErrFull(BADPTR, "Operation was given an invalid pointer, when it needed a valid address.", invalid_pointer);
_Crispr_makeErrBase(badptr, CRISPR_ERR_BADPTR);
_Crispr_makeErrFull(NULL, "Null pointer given for operation, when operation needed a valid address.", badptr);
_Crispr_makeErrFull(FAULT, "Segmentation fault in userspace", badptr);

_Crispr_makeErrFull(NOMEM, "Not enough memory for operation.", resource);
_Crispr_makeErrType(OBJECT, "Object encountered error.");
_Crispr_makeErrBase(object, CRISPR_ERR_OBJECT);
_Crispr_makeErrFull(INIT, "Operation could not initialize object completely.", object);
_Crispr_makeErrFull(INDEX, "Request out of range for object.", object);

_Crispr_makeErrType(MATH, "Operation encountered mathematical error.");
_Crispr_makeErrBase(math, CRISPR_ERR_MATH);
_Crispr_makeErrFull(DOMAIN, "Parameter outside of mathematical domain.", math);
_Crispr_makeErrFull(RANGE, "Overflow encountered in result of operation.", math);

_Crispr_makeErrFull(TYPE, "Operation encountered type error.", object);
_Crispr_makeErrBase(type, CRISPR_ERR_TYPE);
_Crispr_makeErrFull(CONV, "Operation could not convert type into another.", type);
_Crispr_makeErrFull(ATTR, "Type had attributes that were missing or inappropriate for operation.", type);
_Crispr_makeErrFull(EMPTY, "Object was empty, when a non-empty object was expected.", object);
_Crispr_makeErrFull(NOTEMPTY, "Object had things in it, when an empty object was expected.", object);

_Crispr_makeErrType(CHNG, "Operation was interrupted by a change.");
_Crispr_makeErrBase(change, CRISPR_ERR_CHNG);
_Crispr_makeErrBase(signal_pre, CRISPR_ERR_CHNG, CRISPR_ERR_SYS);
_Crispr_makeErrFull(SIG, "Operation was interrupted by a system signal.", signal_pre);
_Crispr_makeErrType(LIMIT, "Operation ran into a limit, and stopped.");
_Crispr_makeErrBase(timeout_pre, CRISPR_ERR_CHNG, CRISPR_ERR_LIMIT);
_Crispr_makeErrFull(TIMEDOUT, "Operation was timed out.", timeout_pre);
_Crispr_makeErrFull(STALE, "Object is currently unusable for operation.", object);
_Crispr_makeErrBase(dead_pre, CRISPR_ERR_CHNG, CRISPR_ERR_STALE, CRISPR_ERR_OBJECT);
_Crispr_makeErrFull(DEAD, "Object became stale during operation.", dead_pre);
_Crispr_makeErrFull(AGAIN, "Operation could not be preformed at this time.", change);

_Crispr_makeErrFull(ACCESS, "Operation cannot access object", object);
_Crispr_makeErrBase(access, CRISPR_ERR_ACCESS);
_Crispr_makeErrFull(PERM, "Operation does not have adequate permissions to use object.", access);

DLL_RESTORE
