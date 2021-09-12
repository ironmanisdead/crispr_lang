#pragma once
#include "predefs.h"
DLL_HIDE

struct _Crispr_Error {
	const char* restrict errdata;
	const Crispr_Errno* restrict bases;
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_Error
#endif

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
DLL_PUBLIC extern const Crispr_Error _Crispr_cn_ERRINTR;
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
#define CRISPR_ERRINTR &_Crispr_cn_ERRINTR //Operation was interrupted.
#define CRISPR_ERRTIMEDOUT &_Crispr_cn_ERRTIMEDOUT //Operation timed out.
#define CRISPR_ERRSTALE &_Crispr_cn_ERRSTALE //Object is already been used up.
#define CRISPR_ERRDEAD &_Crispr_cn_ERRDEAD //Object died during operation.
#define CRISPR_ERRACCESS &_Crispr_cn_ERRACCESS //Operation could not be preformed.
#define CRISPR_ERRPERM &_Crispr_cn_ERRPERM //Operation does not have high enough permissions.
#define CRISPR_ERRAGAIN &_Crispr_cn_ERRAGAIN //Operation could not be preformed at this time.

DLL_PUBLIC const char* Crispr_errName(Crispr_Errno err);
DLL_PUBLIC const char* Crispr_errDesc(Crispr_Errno err);

DLL_PUBLIC const Crispr_Errno* Crispr_errBases(Crispr_Errno err);
DLL_PUBLIC bool Crispr_errIsA(Crispr_Errno err, Crispr_Errno cmp);

DLL_RESTORE
