#pragma once
#include "predefs.h"
#ifdef DLL_OS_unix
 #include <dirent.h>
#else
 #include <fileapi.h>
#endif

typedef struct {
	char access; //file open mode (read/write/exec/etc...)
	char type; //stream, regular, directory, etc.
#ifdef DLL_OS_unix
	union {
		int desc;
		DIR* dir;
	} handle;
#else
	HANDLE handle;
#endif
} Crispr_File;

typedef char Crispr_FileMode;

#define CRISPR_FILE_CLOEXEC 0x01
#define CRISPR_FILE_CREATE 0x02
#define CRISPR_FILE_APPEND 0x04
#define CRISPR_FILE_DIR 0x08
#define CRISPR_FILE_READ 0x10
#define CRISPR_FILE_WRITE 0x20
#define CRISPR_FILE_EXEC 0x40
