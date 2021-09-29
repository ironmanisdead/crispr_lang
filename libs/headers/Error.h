#pragma once
#include "predefs.h"
#include "Memory.h"
DLL_HIDE

struct _Crispr_ErrData {
	bool alloc;
	union {
		const char* restrict errconst;
		char* restrict erralloc;
	};
};

struct _Crispr_Error {
	struct _Crispr_ErrData data;
	const Crispr_Errno* restrict bases;
};

DLL_PUBLIC Dll_NonNull(1, 2) char* _Crispr_errSymMake(const char* restrict name, const char* restrict desc,
		Crispr_Errno* restrict err);

DLL_PUBLIC Dll_NonNull(1) bool _Crispr_errSet(Crispr_Error* restrict dest, struct _Crispr_ErrData data,
		const Crispr_Errno* restrict bases, Crispr_Errno* restrict err);

DLL_PUBLIC Dll_NonNull(1) bool _Crispr_errDynFree(Crispr_Error* restrict obj, Crispr_Errno* restrict err);

#define Crispr_errBaseDef(name, ...) Crispr_Errno crispr_errbaseuser_##name[] = { __VA_ARGS__, Crispr_nullRef(Crispr_Error) }

#define Crispr_errExt(var) extern Crispr_Error crispr_customerr_##var

#define Crispr_errErrno(var) &crispr_customerr_##var

#define Crispr_errToBases(name, errno) const Crispr_Errno* restrict crispr_errbaseuser_##name = errno

#define Crispr_errDefAs(var, name, desc) Crispr_Error crispr_customerr_##var = { { false, \
	{ .errconst = #name "\0" desc } }, \
	Crispr_nullRef(Crispr_Errno) }

#define Crispr_errSetAs(var, name, desc, err) _Crispr_errSet(&(var), \
		(struct _Crispr_ErrData){ false, { .errconst = #name "\0" desc } }, Crispr_nullRef(Crispr_Errno), err)

#define Crispr_errDefFrom(var, name, desc, base) Crispr_Error crispr_customerr_##var = { { false, \
	{ .errconst = #name "\0" desc } }, \
	crispr_errbaseuser_##base }

#define Crispr_errSetFrom(var, name, desc, base, err) _Crispr_errSet(&(var), \
		(struct _Crispr_ErrData){ false, { .errconst = #name "\0" desc } }, \
		crispr_errbaseuser_##base, err)

#define Crispr_errDynDefAs(var, name, desc, err) Crispr_Error crispr_customerr_##var = { \
		{ true, { .erralloc = _Crispr_errSymMake(name, desc, err) } }, Crispr_nullRef(Crispr_Errno) }

#define Crispr_errDynSetAs(var, name, desc, err) _Crispr_errSet(&(var), \
		(struct _Crispr_ErrData){ true, { .erralloc = _Crispr_errSymMake(name, desc, err) } }, \
		Crispr_nullRef(Crispr_Errno), err)

#define Crispr_errDynDefFrom(var, name, desc, base, err) Crispr_Error crispr_customerr_##var = { { true, \
	{ .erralloc = _Crispr_errSymMake(name, desc, err) } }, \
	crispr_errbaseuser_##base }

#define Crispr_errDynSetFrom(var, name, desc, base, err) _Crispr_errSet(&(var), \
		(struct _Crispr_ErrData){ true, { .erralloc = _Crispr_errSymMake(name, desc, err) } }, \
		crispr_errbaseuser_##base, err)

#define Crispr_errDynFree(var, err) _Crispr_errDynFree(&crispr_customerr_##var, err)
#define Crispr_errDynSetFree(var, err) _Crispr_errDynFree(&var, err)

#ifdef __GNUC__
 #pragma GCC poison _Crispr_Error
 #ifndef DLL_EXPORT_LIB_crispr_api
  #pragma GCC poison _Crispr_errDynSetFree
  #pragma GCC poison _Crispr_ErrData
  #pragma GCC poison _Crispr_errSymMake
 #endif
#endif


DLL_PUBLIC const char* Crispr_errName(Crispr_Errno err);
DLL_PUBLIC const char* Crispr_errDesc(Crispr_Errno err);

DLL_PUBLIC const Crispr_Errno* Crispr_errBases(Crispr_Errno err);
DLL_PUBLIC bool Crispr_errIsA(Crispr_Errno err, Crispr_Errno cmp);

DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_SYS;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_RESOURCE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_INVAL;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_REF;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_SYMBOL;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_BADPTR;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_NULL;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_NOMEM;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_OBJECT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_INIT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_INDEX;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_MATH;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_DOMAIN;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_RANGE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_FAULT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_TYPE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_CONV;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_ATTR;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_EMPTY;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_NOEMPTY;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_CHNG;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_SIG;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_LIMIT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_TIMEDOUT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_STALE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_DEAD;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_ACCESS;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_PERM;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_AGAIN;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_VM;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_VM_API;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_VM_SYN;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_VM_SYMREF;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_VM_OP;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERR_VM_ARG;

#define CRISPR_ERR_NOERR ((Crispr_Errno)0) //No error.
#define CRISPR_ERR_SYS &_Crispr_cn_ERR_SYS //System error.
#define CRISPR_ERR_RESOURCE &_Crispr_cn_ERR_RESOURCE //System resource error.
#define CRISPR_ERR_INVAL &_Crispr_cn_ERR_INVAL //Invalid parameter passed to function.
#define CRISPR_ERR_SYMBOL &_Crispr_cn_ERR_SYMBOL //Invalid name for a variable
#define CRISPR_ERR_REF &_Crispr_cn_ERR_REF //Reference error
#define CRISPR_ERR_BADPTR &_Crispr_cn_ERR_BADPTR //Invalid pointer was not expected in operation.
#define CRISPR_ERR_NULL &_Crispr_cn_ERR_NULL //Null parameter was not expected in function.
#define CRISPR_ERR_FAULT &_Crispr_cn_ERR_FAULT //Segmentation fault in userspace.
#define CRISPR_ERR_NOMEM &_Crispr_cn_ERR_NOMEM //No more memory available for operation.
#define CRISPR_ERR_OBJECT &_Crispr_cn_ERR_OBJECT //Object error.
#define CRISPR_ERR_INIT &_Crispr_cn_ERR_INIT //Object initialization failed.
#define CRISPR_ERR_INDEX &_Crispr_cn_ERR_INDEX //Parameter describes index that does not occur in object.
#define CRISPR_ERR_MATH &_Crispr_cn_ERR_MATH //Mathematical error.
#define CRISPR_ERR_DOMAIN &_Crispr_cn_ERR_DOMAIN //Parameter outside mathematical domain.
#define CRISPR_ERR_RANGE &_Crispr_cn_ERR_RANGE //Result cannot be represented due to overflow.
#define CRISPR_ERR_TYPE &_Crispr_cn_ERR_TYPE //Unknown type error.
#define CRISPR_ERR_CONV &_Crispr_cn_ERR_CONV //Type conversion error.
#define CRISPR_ERR_ATTR &_Crispr_cn_ERR_ATTR //Type attribute missing or inappropriate for operation.
#define CRISPR_ERR_EMPTY &_Crispr_cn_ERR_EMPTY //Object was empty, when an un-empty object was expected.
#define CRISPR_ERR_NOEMPTY &_Crispr_cn_ERR_NOEMPTY //Object was not empty, when an empty object was expected.
#define CRISPR_ERR_CHNG &_Crispr_cn_ERR_CHNG //Operation could not complete due to a change.
#define CRISPR_ERR_SIG &_Crispr_cn_ERR_SIG //Operation interrupted due to signal.
#define CRISPR_ERR_LIMIT &_Crispr_cn_ERR_LIMIT //Operation reached specific limit
#define CRISPR_ERR_TIMEDOUT &_Crispr_cn_ERR_TIMEDOUT //Operation timed out.
#define CRISPR_ERR_STALE &_Crispr_cn_ERR_STALE //Object is already been used up.
#define CRISPR_ERR_DEAD &_Crispr_cn_ERR_DEAD //Object died during operation.
#define CRISPR_ERR_ACCESS &_Crispr_cn_ERR_ACCESS //Operation could not be preformed.
#define CRISPR_ERR_PERM &_Crispr_cn_ERR_PERM //Operation does not have high enough permissions.
#define CRISPR_ERR_AGAIN &_Crispr_cn_ERR_AGAIN //Operation could not be preformed at this time.
#define CRISPR_ERR_VM &_Crispr_cn_ERR_VM //VM instruction execution error
#define CRISPR_ERR_VM_API &_Crispr_cn_ERR_VM_API //VM api function error
#define CRISPR_ERR_VM_SYN &_Crispr_cn_ERR_VM_SYN //VM syntax error
#define CRISPR_ERR_VM_SYMREF &_Crispr_cn_ERR_VM_SYMREF //VM symbol error
#define CRISPR_ERR_VM_OP &_Crispr_cn_ERR_VM_OP //VM invalid operand
#define CRISPR_ERR_VM_ARG &_Crispr_cn_ERR_VM_ARG //VM invalid argument

DLL_RESTORE
