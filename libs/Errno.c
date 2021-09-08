#include "headers/Errno.h"

DLL_HIDE

DLL_PUBLIC const char* Crispr_errno(Crispr_Errno stat) {
	switch (stat) {
		case CRISPR_ERRNOERR:
			return "Success.";
		case CRISPR_ERRINVAL:
			return "Invalid parameters.";
		case CRISPR_ERRNULL:
			return "Null pointer error.";
		case CRISPR_ERRNOMEM:
			return "Memory allocation failed.";
		case CRISPR_ERRINIT:
			return "Error in initialization.";
		case CRISPR_ERRINDEX:
			return "Number does not represent valid index in object.";
		case CRISPR_ERRDOMAIN:
			return "Parameter outside mathematical domain.";
		case CRISPR_ERRRANGE:
			return "Mathematical overflow during operation.";
		case CRISPR_ERRFAULT:
			return "Memory fault in userspace.";
		case CRISPR_ERRBADPTR:
			return "Unknown pointer in crispr-api.";
		case CRISPR_ERRTYPE:
			return "Unknown type error.";
		case CRISPR_ERRCONV:
			return "Type/data conversion error.";
		case CRISPR_ERRATTR:
			return "Type/data attribute missing, or incorrect.";
		case CRISPR_ERREMPTY:
			return "Object is empty, when the operation requires a non-empty object.";
		case CRISPR_ERRNOEMPTY:
			return "Object is not empty, when then operation requires an empty object.";
		case CRISPR_ERRINTR:
			return "Interrupted action.";
		case CRISPR_ERRTIMEDOUT:
			return "Action was timed out.";
		case CRISPR_ERRSTALE:
			return "Reference is unusable.";
		case CRISPR_ERRDEAD:
			return "Reference became unusable mid-operation";
		case CRISPR_ERRACCESS:
			return "Access denied.";
		case CRISPR_ERRPERM:
			return "Operation did not have high enough permissions.";
		case CRISPR_ERRAGAIN:
			return "Operation could not be preformed at this time.";
		case CRISPR_ERRUNKNOWN:
		default:
			return "Unknown Error.";
	}
}

DLL_RESTORE
