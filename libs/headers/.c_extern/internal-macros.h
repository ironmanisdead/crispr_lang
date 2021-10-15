#include "macro.h"
#undef DLL_PUBLIC
#undef DLL_LOCAL
#undef DLL_HIDE
#undef DLL_RESTORE
#undef DLL_OS_UNIX
#undef DLL_OS_WINDOWS
#undef DLL_CC_CLANG
#undef DLL_CC_GCC
#if defined(__unix__)
 #define DLL_OS_UNIX 1
#elif defined(_MSC_VER)
 #define DLL_OS_WINDOWS 1
 #if (DLL_EXPORTED > 0)
  #define DLL_PUBLIC __declspec(dllexport)
 #else
  #define DLL_PUBLIC __declspec(dllimport)
 #endif
 #define DLL_LOCAL
#else
 #error "cannot detect operating system used for compilation, only supports Windows and **nix"
#endif
#ifdef __clang__
 #ifndef __GNUC__
  #define DLL_HIDE
  #define DLL_RESTORE
 #endif
 #ifndef DLL_CC_CLANG
  #define DLL_CC_CLANG 1
 #endif
 #ifndef DLL_OS_WINDOWS
  #define DLL_PUBLIC __attribute__ ((visibility("default")))
  #define DLL_LOCAL __attribute__ ((visibility("hidden")))
 #endif
#endif
#ifdef __GNUC__
 #define DLL_HIDE _Pragma("GCC visibility push (internal)")
 #define DLL_RESTORE _Pragma("GCC visibility pop")
 #define DLL_CC_GCC 1
 #ifndef DLL_OS_WINDOWS
  #define DLL_PUBLIC __attribute__ ((visibility("default")))
  #define DLL_LOCAL __attribute__ ((visibility("hidden")))
 #endif
#else
 #define DLL_HIDE
 #define DLL_RESTORE
 #ifndef DLL_OS_WINDOWS
  #define DLL_PUBLIC
  #define DLL_LOCAL
 #endif
#endif
#if (defined __cplusplus) && ! (defined RECAST)
 #define RECAST(type, exp) reinterpret_cast<type>(exp)
#endif
#ifndef DEPRECATE
 #if defined (__GNUC__)
  #define DEPRECATE(fun, msg) fun __attribute__((deprecated(msg)))
 #elif define (_MSC_VER)
  #define DEPRECATE(fun, msg) __declspec(deprecated(msg)) fun
 #endif
#endif
#ifdef __cplusplus
	#define Dll_Init(name, attr) \
		static void name(void) attr; \
		struct name##_t_ { name##_t_(void) { name(); } }; static name##_t_ name##_; \
		static void name(void) attr
#elif defined(_MSC_VER)
	#pragma section(".CRT$XCU",read)
	#define INITIALIZER2_(name, p, attr) \
		static void name(void) attr; \
		__declspec(allocate(".CRT$XCU")) void (*name##_)(void) = name; \
		__pragma(comment(linker,"/include:" p #name "_")) \
		static void name(void) attr
	#ifdef _WIN64
		#define Dll_Init(name, attr) INITIALIZER2_(name, "", attr)
	#else
		#define Dll_Init(name, attr) INITIALIZER2_(name, "_", attr)
	#endif
#else
	#define Dll_Init(name, attr) \
		static void name(void) __attribute__((constructor)) attr; \
		static void name(void)
#endif
#ifdef DLL_CC_GCC
 #define Dll_NonNull(...) __attribute__ ((nonnull(__VA_ARGS__)))
#else
 #define Dll_NonNull(...)
#endif
#ifdef DLL_CC_GCC
 #define Dll_Enum __attribute__((packed))
#else
 #define Dll_Enum
#endif
#ifdef DLL_OS_WINDOWS
 #define Dll_pragma(act) __pragma(#act) extern int Dll_NoOpN
#else
 #define Dll_pragma(act) _Pragma(#act) extern int Dll_NoOpN
#endif
#define Dll_pack(...) Dll_pragma(pack(__VA_ARGS__))
#define DLL_ALIGN Dll_pragma(pack())
#ifndef Utils_offset
 #define Utils_offset(type, field) ((decltype(sizeof 0))(&((type*)0)->field))
#endif
