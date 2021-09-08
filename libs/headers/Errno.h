#pragma once
#include "predefs.h"
DLL_HIDE

#define CRISPR_ERRNOERR 0ul //No error.
#define CRISPR_ERRUNKNOWN 1ul //Unknown error.
#define CRISPR_ERRINVAL 2ul //Invalid parameter passed to function.
#define CRISPR_ERRSYMBOL 3ul //Invalid name for a variable
#define CRISPR_ERRNULL 4ul //Null parameter not expected in function.
#define CRISPR_ERRNOMEM 5ul //No more memory available for operation.
#define CRISPR_ERRINIT 6ul //Object initialization failed.
#define CRISPR_ERRINDEX 7ul //Parameter describes index that does not occur in object.
#define CRISPR_ERRDOMAIN 8ul //Parameter outside mathematical domain.
#define CRISPR_ERRRANGE 9ul //Result cannot be represented due to overflow.
#define CRISPR_ERRFAULT 10ul //Segmentation fault in userspace.
#define CRISPR_ERRBADPTR 11ul //API did not recognize pointer.
#define CRISPR_ERRTYPE 12ul //Unknown type error.
#define CRISPR_ERRCONV 13ul //Type conversion error.
#define CRISPR_ERRATTR 14ul //Type attribute missing or inappropriate for operation.
#define CRISPR_ERREMPTY 15ul //Object was empty, when an un-empty object was expected.
#define CRISPR_ERRNOEMPTY 16ul //Object was not empty, when an empty object was expected.
#define CRISPR_ERRINTR 17ul //Operation was interrupted.
#define CRISPR_ERRTIMEDOUT 18ul //Operation timed out.
#define CRISPR_ERRSTALE 19ul //Object is already been used up.
#define CRISPR_ERRDEAD 20ul //Object died during operation.
#define CRISPR_ERRACCESS 21ul //Object was inaccessible.
#define CRISPR_ERRPERM 22ul //Operation does not have high enough permissions.
#define CRISPR_ERRAGAIN 23ul //Operation could not be preformed at this time.

DLL_PUBLIC const char* Crispr_errno(Crispr_Errno);

DLL_RESTORE
