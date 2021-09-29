#pragma once
#include "predefs.h"
#ifdef DLL_OS_UNIX
 #include <dirent.h>
#else
 #include <fileapi.h>
#endif

DLL_HIDE

typedef struct {
	char type; //stream, regular, directory, etc.
	char access; //file open mode (read/write/exec/etc...)
#ifdef DLL_OS_UNIX
	union {
		int desc;
		DIR* dir;
	} handle;
#else
	HANDLE handle;
#endif
} Crispr_File;

//CRISPR_FILE_ macros specify the access modes to open the file, and file types for creating a file
#define CRISPR_FILE_CLOEXEC 0x0001
#define CRISPR_FILE_CREATE 0x0002
#define CRISPR_FILE_APPEND 0x0004
#define CRISPR_FILE_TRUNC 0x0008
#define CRISPR_FILE_RM 0x000C // (FILE_APPEND | FILE_TRUNC)
#define CRISPR_FILE_TMP 0x000E // (FILE_CREATE | FILE_RM)
#define CRISPR_FILE_PIPE 0x0010
#define CRISPR_FILE_DIR 0x0020
#define CRISPR_FILE_EXEC 0x0040
#define CRISPR_FILE_READ 0x0080
#define CRISPR_FILE_WRITE 0x0100
#define CRISPR_FILE_RDWR 0x0180 // (FILE_READ | FILE_WRITE)
#define CRISPR_FILE_RDWX 0x01C0 // (FILE_READ | FILE_WRITE | FILE_EXEC)

//CRISPR_FMODE_ macros specify the permissions changed or created for the file.
#define CRISPR_FMODE_EXEC_UNSET 0x01
#define CRISPR_FMODE_EXEC_SET 0x02
#define CRISPR_FMODE_EXEC_FLIP 0x03
#define CRISPR_FMODE_READ_UNSET 0x04
#define CRISPR_FMODE_READ_SET 0x08
#define CRISPR_FMODE_READ_FLIP 0x0C
#define CRISPR_FMODE_WRITE_UNSET 0x10
#define CRISPR_FMODE_WRITE_SET 0x20
#define CRISPR_FMODE_WRITE_FLIP 0x30
#define CRISPR_FMODE_RDONLY 0x19 // (EXEC_UNSET | READ_SET | WRITE_UNSET)
#define CRISPR_FMODE_RDWR 0x29 // (EXEC_UNSET | READ_SET | WRITE_SET)
#define CRISPR_FMODE_RDWX 0x2C // (EXEC_SET | READ_SET | WRITE_SET)
#define CRISPR_FMODE_EXONLY 0x16 // (EXEC_SET | READ_UNSET | WRITE_UNSET)

DLL_PUBLIC bool Crispr_fileUse(Crispr_File* restrict dst, const char* restrict name,
		short mode, short perm, Crispr_Errno* restrict err);
DLL_PUBLIC bool Crispr_fileClose(Crispr_File* restrict target, Crispr_Errno* restrict err);

DLL_RESTORE
