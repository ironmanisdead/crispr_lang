#pragma once
#include "predefs.h"
DLL_HIDE

#define CRISPR_ERRNOERR 0ul //No error.
#define CRISPR_ERRUNKNOWN 1ul //Unknown error.
#define CRISPR_ERRINVAL 2ul //Invalid parameter passed to function.
#define CRISPR_ERRNULL 3ul //Null parameter not expected in function.
#define CRISPR_ERRNOMEM 4ul //No more memory available for operation.
#define CRISPR_ERRINIT 5ul //Object initialization failed.
#define CRISPR_ERRINDEX 6ul //Parameter describes index that does not occur in object.
#define CRISPR_ERRDOMAIN 7ul //Parameter outside mathematical domain.
#define CRISPR_ERRRANGE 8ul //Result cannot be represented due to overflow.
#define CRISPR_ERRFAULT 9ul //Segmentation fault in userspace.
#define CRISPR_ERRBADPTR 10ul //API did not recognize pointer.
#define CRISPR_ERRTYPE 11ul //Unknown type error.
#define CRISPR_ERRCONV 12ul //Type conversion error.
#define CRISPR_ERRATTR 13ul //Type attribute missing or inappropriate for operation.
#define CRISPR_ERREMPTY 14ul //Object was empty, when an un-empty object was expected.
#define CRISPR_ERRNOEMPTY 15ul //Object was not empty, when an empty object was expected.
#define CRISPR_ERRINTR 16ul //Operation was interrupted.
#define CRISPR_ERRTIMEDOUT 17ul //Operation timed out.
#define CRISPR_ERRSTALE 18ul //Object is already been used up.
#define CRISPR_ERRDEAD 19ul //Object died during operation.
#define CRISPR_ERRACCESS 20ul //Object was inaccessible.
#define CRISPR_ERRPERM 21ul //Operation does not have high enough permissions.
#define CRISPR_ERRAGAIN 22ul //Operation could not be preformed at this time.

DLL_PUBLIC const char* Crispr_errno(Crispr_Errno);

DLL_RESTORE
