#pragma once
#include "Status.h"

DLL_PUBLIC Crispr_Errno Crispr_enter_Namespace(const char* restrict);
DLL_PUBLIC Crispr_Errno Crispr_exit_Namespace();

DLL_PUBLIC Crispr_Errno Crispr_use_Namespace(const char* restrict);
