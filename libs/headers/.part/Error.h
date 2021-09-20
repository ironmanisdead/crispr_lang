#pragma once
#include "../predefs.h"

#define _Crispr_makeErrType(name, info) DLL_PUBLIC const Crispr_Error _Crispr_cn_ERR##name = \
	{ { false, { .errconst = #name "\0" info } }, Crispr_nullObj(Crispr_Errno) }
#define _Crispr_makeErrBase(name, ...) static const Crispr_Errno crispr_errbase_##name[] = \
	{ __VA_ARGS__, Crispr_nullObj(Crispr_Error) }
#define _Crispr_makeErrFull(name, info, base) DLL_PUBLIC const Crispr_Error _Crispr_cn_ERR##name = \
	{ { false, { .errconst = #name "\0" info } }, crispr_errbase_##base }
