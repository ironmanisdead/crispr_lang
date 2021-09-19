#pragma once
#include "predefs.h"
#ifdef DLL_OS_unix
 #include <dirent.h>
#else
 #include <fileapi.h>
#endif

DLL_HIDE

typedef struct {
	char type; //stream, regular, directory, etc.
	char access; //file open mode (read/write/exec/etc...)
#ifdef DLL_OS_unix
	union {
		int desc;
		DIR* dir;
	} handle;
#else
	HANDLE handle;
#endif
} Crispr_File;

//CRISPR_FILE_ macros specify the permissions and access mode to give to the file, or to open the file as.
#define CRISPR_FILE_CLOEXEC 0x01
#define CRISPR_FILE_CREATE 0x02
#define CRISPR_FILE_APPEND 0x04
#define CRISPR_FILE_DIR 0x08
#define CRISPR_FILE_PIPE 0x0C
#define CRISPR_FILE_READ 0x10
#define CRISPR_FILE_WRITE 0x20
#define CRISPR_FILE_EXEC 0x40

DLL_PUBLIC bool Crispr_fileOpen(Crispr_File* restrict dst, const char* restrict name,
		char mode, Crispr_Errno* restrict err);
DLL_PUBLIC bool Crispr_fileClose(Crispr_File* restrict target, Crispr_Errno* restrict err);

DLL_RESTORE
