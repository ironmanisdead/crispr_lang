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

DLL_PUBLIC char* _Crispr_errSymMake(const char* restrict name, const char* restrict desc, Crispr_Errno* restrict err) {
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

DLL_PUBLIC bool _Crispr_errSet(Crispr_Error* restrict res, struct _Crispr_ErrData data,
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

DLL_PUBLIC bool _Crispr_errDynFree(Crispr_Error* restrict target, Crispr_Errno* restrict err) {
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
