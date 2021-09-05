#include "headers/Errno.h"

DLL_HIDE

DLL_PUBLIC const char* Crispr_f_Errno(Crispr_tn_Errno stat) {
	switch (stat) {
		case Crispr_cn_ENOERR:
			return "Success.";
		case Crispr_cn_EINVAL:
			return "Invalid parameters.";
		case Crispr_cn_ENULL:
			return "Null pointer error.";
		case Crispr_cn_ENOMEM:
			return "Memory allocation failed.";
		case Crispr_cn_EINIT:
			return "Error in initialization.";
		case Crispr_cn_ERANGE:
			return "Parameter out of range.";
		case Crispr_cn_EDOMAIN:
			return "Parameter outside mathematical domain.";
		case Crispr_cn_EFAULT:
			return "Memory fault in userspace.";
		case Crispr_cn_EBADPTR:
			return "Unknown pointer in cextensions.";
		case Crispr_cn_ETYPE:
			return "Unknown type error.";
		case Crispr_cn_ECONV:
			return "Type/data conversion error.";
		case Crispr_cn_EATTR:
			return "Type/data attribute missing, or incorrect.";
		case Crispr_cn_EEMPTY:
			return "Object is empty.";
		case Crispr_cn_EINTR:
			return "Interrupted action.";
		case Crispr_cn_ETIMEDOUT:
			return "Action was timed out.";
		case Crispr_cn_ESTALE:
			return "Reference is unusable.";
		case Crispr_cn_EDEAD:
			return "Reference became unusable mid-operation";
		case Crispr_cn_EACCESS:
			return "Access denied.";
		case Crispr_cn_EAGAIN:
			return "Operation could not be preformed at this time.";
		case Crispr_cn_EUNKNOWN:
		default:
			return "Unknown Error.";
	}
}

DLL_RESTORE
