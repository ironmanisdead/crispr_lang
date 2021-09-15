#pragma once
#include "predefs.h"
#include "Memory.h"
DLL_HIDE

struct _Crispr_ErrData {
	bool fixed;
	union {
		const char* restrict errconst;
		char* restrict erralloc;
	};
};

struct _Crispr_Error {
	struct _Crispr_ErrData data;
	const Crispr_Errno* restrict bases;
};

DLL_PUBLIC char* _Crispr_errSymMake(const char* restrict name, const char* restrict desc,
		Crispr_Errno* restrict err);

DLL_PUBLIC Crispr_Error _Crispr_errMake(struct _Crispr_ErrData info,
		const Crispr_Errno* restrict base, Crispr_Errno* restrict err);

#define Crispr_errBaseDef(name, ...) Crispr_Errno crispr_errbases_##name[] = { __VA_ARGS__, CRISPR_NULL }
#define Crispr_errInitAs(var, name, info, err) Crispr_Error var = _Crispr_errMake(\
		{ true, { .errconst = #name "\0" info } }, CRISPR_NULL, err)
#define Crispr_errInitFrom(var, name, info, base, err) Crispr_Error var = _Crispr_errMake(\
		{ true, { .errconst = #name "\0" info }, crispr_errbases_##base, err)

#define Crispr_errDynInitAs(var, name, info, err) Crispr_Error var = _Crispr_errMake(\
		{ false, { .erralloc = _Crispr_errSymMake(name, info, err) }, CRISPR_NULL, err)
#define Crispr_errDynFree(var) Crispr_free((char*)(var).errdata)

#ifdef __GNUC__
 #pragma GCC poison _Crispr_Error
 #pragma GCC poison _Crispr_errSymMake
 #pragma GCC poison _Crispr_errMake
#endif

DLL_PUBLIC const char* Crispr_errName(Crispr_Errno err);
DLL_PUBLIC const char* Crispr_errDesc(Crispr_Errno err);

DLL_PUBLIC const Crispr_Errno* Crispr_errBases(Crispr_Errno err);
DLL_PUBLIC bool Crispr_errIsA(Crispr_Errno err, Crispr_Errno cmp);

DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRSYS;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRRESOURCE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRINVAL;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRSYMBOL;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRBADPTR;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRNULL;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRNOMEM;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERROBJECT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRINIT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRINDEX;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRMATH;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRDOMAIN;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRRANGE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRFAULT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRTYPE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRCONV;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRATTR;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERREMPTY;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRNOEMPTY;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRCHNG;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRSIG;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRTIMEDOUT;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRSTALE;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRDEAD;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRACCESS;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRPERM;
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRAGAIN;

#define CRISPR_ERRNOERR ((Crispr_Errno)0) //No error.
#define CRISPR_ERRSYS &_Crispr_cn_ERRSYS //System error.
#define CRISPR_ERRRESOURCE &_Crispr_cn_ERRRESOURCE //System resource error.
#define CRISPR_ERRINVAL &_Crispr_cn_ERRINVAL //Invalid parameter passed to function.
#define CRISPR_ERRSYMBOL &_Crispr_cn_ERRSYMBOL //Invalid name for a variable
#define CRISPR_ERRBADPTR &_Crispr_cn_ERRBADPTR //Invalid pointer was not expected in operation.
#define CRISPR_ERRNULL &_Crispr_cn_ERRNULL //Null parameter was not expected in function.
#define CRISPR_ERRFAULT &_Crispr_cn_ERRFAULT //Segmentation fault in userspace.
#define CRISPR_ERRNOMEM &_Crispr_cn_ERRNOMEM //No more memory available for operation.
#define CRISPR_ERROBJECT &_Crispr_cn_ERROBJECT //Object error.
#define CRISPR_ERRINIT &_Crispr_cn_ERRINIT //Object initialization failed.
#define CRISPR_ERRINDEX &_Crispr_cn_ERRINDEX //Parameter describes index that does not occur in object.
#define CRISPR_ERRMATH &_Crispr_cn_ERRMATH //Mathematical error.
#define CRISPR_ERRDOMAIN &_Crispr_cn_ERRDOMAIN //Parameter outside mathematical domain.
#define CRISPR_ERRRANGE &_Crispr_cn_ERRRANGE //Result cannot be represented due to overflow.
#define CRISPR_ERRTYPE &_Crispr_cn_ERRTYPE //Unknown type error.
#define CRISPR_ERRCONV &_Crispr_cn_ERRCONV //Type conversion error.
#define CRISPR_ERRATTR &_Crispr_cn_ERRATTR //Type attribute missing or inappropriate for operation.
#define CRISPR_ERREMPTY &_Crispr_cn_ERREMPTY //Object was empty, when an un-empty object was expected.
#define CRISPR_ERRNOEMPTY &_Crispr_cn_ERRNOEMPTY //Object was not empty, when an empty object was expected.
#define CRISPR_ERRCHNG &_Crispr_cn_ERRCHNG //Operation could not complete due to a change.
#define CRISPR_ERRSIG &_Crispr_cn_ERRSIG //Operation interrupted due to signal.
#define CRISPR_ERRTIMEDOUT &_Crispr_cn_ERRTIMEDOUT //Operation timed out.
#define CRISPR_ERRSTALE &_Crispr_cn_ERRSTALE //Object is already been used up.
#define CRISPR_ERRDEAD &_Crispr_cn_ERRDEAD //Object died during operation.
#define CRISPR_ERRACCESS &_Crispr_cn_ERRACCESS //Operation could not be preformed.
#define CRISPR_ERRPERM &_Crispr_cn_ERRPERM //Operation does not have high enough permissions.
#define CRISPR_ERRAGAIN &_Crispr_cn_ERRAGAIN //Operation could not be preformed at this time.

DLL_RESTORE
