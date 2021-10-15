#ifndef _DLL_MACRO
#define _DLL_MACRO 1
#define Macro_false 0
#define Macro_true 1
#define _Macro_if0(tru, fail) fail
#define _Macro_if1(tru, fail) tru
#define Macro_cat(fir, sec) fir##sec
#define Macro_if(cond) Macro_cat(_Macro_if, cond)
#endif
