#pragma once

DLL_LOCAL char _Crispr_flagGet(volatile Crispr_Lock* obj, bool wait);
DLL_LOCAL void _Crispr_flagSet(volatile Crispr_Lock* obj, char setting);
