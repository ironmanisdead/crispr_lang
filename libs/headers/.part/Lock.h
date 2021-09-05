#pragma once

DLL_LOCAL char _Crispr_f_FlagGet(volatile Crispr_t_Lock* obj, bool wait);
DLL_LOCAL void _Crispr_f_FlagSet(volatile Crispr_t_Lock* obj, char setting);
