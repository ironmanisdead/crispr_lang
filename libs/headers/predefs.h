#pragma once
#if (defined __SIZE_TYPE__) && (defined __SIZEOF_SIZE_T__)
 typedef __SIZE_TYPE__ Crispr_Size;
 #if (__SIZEOF_SIZE_T__ == 1)
  typedef signed char Crispr_Ssize;
 #elif (__SIZEOF_SIZE_T__ == 2)
  typedef short Crispr_Ssize;
 #elif (__SIZEOF_SIZE_T__ == 4)
  typedef long Crispr_Ssize;
 #elif (__SIZEOF_SIZE_T__ == 8)
  typedef long long Crispr_Ssize;
 #else
  #error unknown compiler size types
 #endif
#else
 #include <stddef.h>
#endif
#ifndef __bool_true_false_are_defined
 #include <stdbool.h>
#endif
typedef signed char Crispr_Schar;
typedef unsigned char Crispr_Uchar;
typedef unsigned short Crispr_Ushort;
typedef unsigned int Crispr_Uint;
typedef unsigned long Crispr_Ulong;
typedef long long Crispr_S64;
typedef unsigned long long Crispr_U64;
typedef struct _Crispr_State Crispr_State;
typedef struct _Crispr_NameSpace Crispr_NameSpace;
typedef struct _Crispr_Class Crispr_Class;
typedef struct _Crispr_Reference Crispr_Reference;
typedef struct _Crispr_Method Crispr_Method;
typedef struct _Crispr_Bindings Crispr_Bindings;
typedef struct _Crispr_Field Crispr_Field;
typedef struct _Crispr_Error Crispr_Error;
typedef const Crispr_Error* restrict Crispr_Errno;
#define Crispr_boolStr(exp) ((exp) ? ("(" #exp ") is true") : ("(" #exp ") is false"))
#define CRISPR_NULL (void*)(0)
#define Crispr_nullRef(type) (type*)(0)
#define Crispr_nullFun(ret, ...) (ret (*)(__VA_ARGS__))(0)
#pragma push_macro("DLL_EXPORTED")
#define DLL_EXPORTED DLL_EXPORT_LIB_crispr_api
#include ".c_extern/internal-macros.h"
#pragma pop_macro("DLL_EXPORTED")
#ifndef _GNU_SOURCE
 #define _GNU_SOURCE 1
#endif
