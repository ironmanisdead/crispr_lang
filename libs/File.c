#include "headers/File.h"

DLL_HIDE

DLL_PUBLIC bool Crispr_fileOpen(Crispr_File* restrict dst, const char* restrict name,
		char mode, Crispr_Errno* restrict err);

DLL_PUBLIC bool Crispr_fileClose(Crispr_File* restrict dst, Crispr_Errno* restrict err);
