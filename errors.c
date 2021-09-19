#include <CrisprLang/Error.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	Crispr_errDefAs(err, TEST, "Test error");
	Crispr_Errno test = Crispr_errErrno(err);
	printf("%s: %s\n", Crispr_errName(test), Crispr_errDesc(test));
	Crispr_Error var;
	Crispr_errDynSetAs(var, "Dynamic", "Dynamic error", CRISPR_NULL);
	printf("%s: %s\n", Crispr_errName(&var), Crispr_errDesc(&var));
	Crispr_errDynSetFree(var, CRISPR_NULL);
	exit(0);
}
