#pragma once
#if (defined __SIZE_TYPE__) && (defined __SIZEOF_SIZE_T__)
 typedef __SIZE_TYPE__ Crispr_t_size;
 #if (__SIZEOF_SIZE_T__ == 1)
  typedef signed char Crispr_t_ssize;
 #elif (__SIZEOF_SIZE_T__ == 2)
  typedef short Crispr_t_ssize;
 #elif (__SIZEOF_SIZE_T__ == 4)
  typedef long Crispr_t_ssize;
 #elif (__SIZEOF_SIZE_T__ == 8)
  typedef long long Crispr_t_ssize;
 #else
  #error unknown compiler size types
 #endif
#else
 #include <stddef.h>
#endif
#ifndef __bool_true_false_are_defined
 #include <stdbool.h>
#endif
typedef signed char Crispr_t_schar;
typedef unsigned short Crispr_t_ushort;
typedef unsigned int Crispr_t_uint;
typedef unsigned long Crispr_t_ulong;
typedef long long Crispr_t_s64;
typedef unsigned long long Crispr_t_u64;
typedef struct _Crispr_t_State Crispr_t_State;
typedef struct _Crispr_t_Namespace Crispr_t_Namespace;
typedef struct _Crispr_t_Class Crispr_t_Class;
typedef struct _Crispr_t_Object Crispr_t_Object;
typedef struct _Crispr_t_Method Crispr_t_Method;
typedef struct _Crispr_t_Bindings Crispr_t_Bindings;
typedef struct _Crispr_t_Field Crispr_t_Field;
typedef enum _Crispr_tn_Errno Crispr_tn_Errno;
#define Crispr_M_NULL (void*)(0)
#pragma push_macro("DLL_EXPORTED")
#define DLL_EXPORTED DLL_EXPORT_LIB_crispr_api
#include ".c_extern/internal-macros.h"
#pragma pop_macro("DLL_EXPORTED")
#ifndef _GNU_SOURCE
 #define _GNU_SOURCE 1
#endif
