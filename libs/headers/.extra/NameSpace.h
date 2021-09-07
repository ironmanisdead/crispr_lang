#pragma once
#include "predefs.h"

struct _Crispr_NameSpace {
	const Crispr_NameSpace* parent;
	char[] name;
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_NameSpace
#endif
