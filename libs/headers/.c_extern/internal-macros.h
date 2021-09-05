#undef DLL_PUBLIC
#undef DLL_LOCAL
#undef DLL_HIDE
#undef DLL_RESTORE
#undef DLL_OS_unix
#undef DLL_OS_windows
#undef DLL_CC_clang
#undef DLL_CC_gcc
#if defined(__unix__)
 #define DLL_OS_unix 1
#elif defined(_MSC_VER)
 #define DLL_OS_windows 1
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
 #ifndef DLL_CC_clang
  #define DLL_CC_clang 1
 #endif
 #ifndef DLL_OS_windows
  #define DLL_PUBLIC __attribute__ ((visibility("default")))
  #define DLL_LOCAL __attribute__ ((visibility("hidden")))
 #endif
#endif
#ifdef __GNUC__
 #define DLL_HIDE _Pragma("GCC visibility push (internal)")
 #define DLL_RESTORE _Pragma("GCC visibility pop")
 #define DLL_CC_gcc 1
 #ifndef DLL_OS_windows
  #define DLL_PUBLIC __attribute__ ((visibility("default")))
  #define DLL_LOCAL __attribute__ ((visibility("hidden")))
 #endif
#else
 #define DLL_HIDE
 #define DLL_RESTORE
 #ifndef DLL_OS_windows
  #define DLL_PUBLIC
  #define DLL_LOCAL
 #endif
#endif
#ifndef RECAST
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
	#define INITIALIZER(f, attr) \
		static void f(void) attr; \
		struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
		static void f(void) attr
#elif defined(_MSC_VER)
	#pragma section(".CRT$XCU",read)
	#define INITIALIZER2_(f, p) \
		static void f(void); \
		__declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
		__pragma(comment(linker,"/include:" p #f "_")) \
		static void f(void)
	#ifdef _WIN64
		#define INITIALIZER(f) INITIALIZER2_(f, "")
	#else
		#define INITIALIZER(f) INITIALIZER2_(f, "_")
	#endif
#else
	#define INITIALIZER(f) \
		static void f(void) __attribute__((constructor)); \
		static void f(void)
#endif
#ifndef Utils_offset
 #define Utils_offset(type, field) ((decltype(sizeof 0))(&((type*)0)->field))
#endif
