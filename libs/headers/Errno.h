#pragma once
#include "predefs.h"
DLL_HIDE

#define CRISPR_ERRNOERR 0ul
#define CRISPR_ERRUNKNOWN 1ul
#define CRISPR_ERRINVAL 2ul
#define CRISPR_ERRNULL 3ul
#define CRISPR_ERRNOMEM 4ul
#define CRISPR_ERRINIT 5ul
#define CRISPR_ERRRANGE 6ul
#define CRISPR_ERRDOMAIN 7ul
#define CRISPR_ERRFAULT 8ul
#define CRISPR_ERRBADPTR 9ul
#define CRISPR_ERRTYPE 10ul
#define CRISPR_ERRCONV 11ul
#define CRISPR_ERRATTR 12ul
#define CRISPR_ERREMPTY 13ul
#define CRISPR_ERRINTR 14ul
#define CRISPR_ERRTIMEDOUT 15ul
#define CRISPR_ERRSTALE 16ul
#define CRISPR_ERRDEAD 17ul
#define CRISPR_ERRACCESS 18ul
#define CRISPR_ERRAGAIN 19ul

DLL_PUBLIC const char* Crispr_errno(Crispr_Errno);

DLL_RESTORE
