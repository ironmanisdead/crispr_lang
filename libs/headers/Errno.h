#pragma once
#include "predefs.h"
DLL_HIDE

enum _Crispr_tn_Errno {
	Crispr_cn_ENOERR, //successful operation
	Crispr_cn_EUNKNOWN, //unknown error
	Crispr_cn_EINVAL, //invalid parameter
	Crispr_cn_ENULL, //value was null
	Crispr_cn_ENOMEM, //memory allocation failed
	Crispr_cn_EINIT, //constructor callback failed
	Crispr_cn_ERANGE, //index out of range
	Crispr_cn_EDOMAIN, //input outside mathematical domain
	Crispr_cn_EFAULT, //memory fault in userspace (rare/debugging)
	Crispr_cn_EBADPTR, //unrecognized memory pointer in cextensions
	Crispr_cn_ETYPE, //unknown/general type error
	Crispr_cn_ECONV, //invalid class/type conversion
	Crispr_cn_EATTR, //missing/invalid type attribute
	Crispr_cn_EEMPTY, //object is empty
	Crispr_cn_EINTR, //interrupted in operation
	Crispr_cn_ETIMEDOUT, //time ran out for operation
	Crispr_cn_ESTALE, //resource already is gone
	Crispr_cn_EDEAD, //resource died during access
	Crispr_cn_EACCESS, //cannot access resource
	Crispr_cn_EAGAIN, //operation did not work at this time
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_tn_Errno
#endif

DLL_PUBLIC const char* Crispr_f_Errno(Crispr_tn_Errno);

DLL_RESTORE
