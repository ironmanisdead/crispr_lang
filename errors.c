#include <CrisprLang/Error.h>
#include <stdio.h>

int main() {
	Crispr_errDefAs(err, TEST, "Test error");
	Crispr_Errno test = Crispr_errErrno(err);
	printf("%s: %s\n", Crispr_errName(test), Crispr_errDesc(test));
	Crispr_Error var;
	Crispr_errSetAs(var, LUKE, "Lucrative error", CRISPR_NULL);
}
