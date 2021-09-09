#pragma once
#include "predefs.h"
#include <threads.h>
DLL_HIDE

typedef struct {
	mtx_t* lock; //NULL if single-thread is assumed
	Crispr_Class** value; //where is the reference stored
} Crispr_ClsRefLck;

typedef struct _Crispr_ClsBaseNode {
	bool single;
	mtx_t lock; //lock only available if polythreaded
	Crispr_Class* base;
	struct _Crispr_ClsBaseNode* next; //Null if current member is last member
} Crispr_ClsBaseNode;

#ifdef __GNUC__
 #pragma GCC poison _Crispr_ClsBaseNode
#endif

struct _Crispr_Class {
	bool proto;
	bool finality;
	const Crispr_NameSpace* ns;
	Crispr_Size namelen;
	Crispr_Size baselen;
	Crispr_Size alignment;
};

#ifdef __GNUC__
 #pragma GCC poison _Crispr_Class
#endif

typedef struct {
	Crispr_Class main;
	Crispr_ClsRefLck* refs;
	Crispr_ClsBaseNode* bases;
	const char* restrict name;
} Crispr_ProtoClass;

DLL_PUBLIC bool Crispr_protoClass(Crispr_ProtoClass* restrict dst, const char* restrict name, Crispr_Errno* restrict err);

DLL_PUBLIC const Crispr_Class* Crispr_newClass(const char* restrict name, Crispr_Errno* restrict);

DLL_RESTORE
