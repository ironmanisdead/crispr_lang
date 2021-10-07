#include "headers/String.h"
#include "headers/Error.h"

DLL_HIDE

DLL_PUBLIC Crispr_Size Crispr_strLen(const char* restrict str) {
	Crispr_Size i = 0;
	while (str[i] != '\0')
		++i;
	return i;
}

DLL_PUBLIC Crispr_Size Crispr_strCpy(char* restrict dst, const char* restrict src) {
	Crispr_Size i = 0;
	while (src[i] != '\0') {
		dst[i] = src[i];
		++i;
	}
	dst[i] = '\0';
	return i;
}

DLL_PUBLIC Crispr_Off Crispr_strIter(const char* restrict str, Crispr_LoopStr loop, Crispr_Errno* restrict err) {
	Crispr_Off len = 0;
	if (err)
		*err = CRISPR_ERR_NOERR;
start:
	switch (loop(str, len, err)) {
		case CRISPR_LOOP_FAIL:
			return -len - 1;
		case CRISPR_LOOP_CONT:
			if (++len < 0) {
				if (err)
					*err = CRISPR_ERR_RANGE;
				return len;
			}
			goto start;
		case CRISPR_LOOP_DONE:
			return len;
		default:
			if (err)
				*err = CRISPR_ERR_INVAL;
			return -len - 1; //two's complement inverse
	}
}

DLL_PUBLIC Crispr_LoopStat Crispr_symName(const char* restrict str, Crispr_Size off, Crispr_Errno* restrict err) {
	if (off == 0) {
		char ch = *str;
		if ( (ch == '_') || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) ) {
			return CRISPR_LOOP_CONT;
		} else {
			if (err)
				*err = CRISPR_ERR_SYMBOL;
			return CRISPR_LOOP_FAIL;
		}
	} else {
		char ch = str[off];
		if (ch == '\0')
			return CRISPR_LOOP_DONE;
		if ( ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_') ||
				((ch >= '0') && (ch <= '9')) ) {
			return CRISPR_LOOP_CONT;
		}
		if (err)
			*err = CRISPR_ERR_SYMBOL;
		return CRISPR_LOOP_FAIL;
	}
}

DLL_PUBLIC Crispr_LoopStat Crispr_symVm(const char* restrict str, Crispr_Size off, Crispr_Errno* restrict err) {
	Crispr_Errno rec = CRISPR_ERR_NOERR;
	if (err)
		rec = *err;
	Crispr_LoopStat stat = Crispr_symVm(str, off, &rec);
	if ((stat == CRISPR_LOOP_FAIL) && (str[off] == '.')) {
		return CRISPR_LOOP_CONT;
	} else {
		if (err)
			*err = rec;
		return stat;
	}
}

DLL_RESTORE
