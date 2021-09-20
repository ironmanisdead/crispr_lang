#include <CrisprLang/Memory.h>
#include <CrisprLang/Timer.h>
#include <CrisprLang/Sema.h>
#include <CrisprLang/Error.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int main() {
	Crispr_Sema mutex;
	Crispr_sema_init(&mutex, 1, Crispr_nullRef(Crispr_Errno));
	Crispr_sema_lock(&mutex, false, Crispr_nullRef(Crispr_Timer), Crispr_nullRef(Crispr_Errno));
	Crispr_Timer timer;
	Crispr_timeNow(&timer, CRISPR_CLK_RELA, 2, CRISPR_TIME_SECOND, Crispr_nullRef(Crispr_Errno));
	Crispr_Timer timelock;
	Crispr_timeConv(&timelock, &timer, CRISPR_CLK_UTC, CRISPR_TIME_SECOND, Crispr_nullRef(Crispr_Errno));
	Crispr_timeConv(&timer, &timelock, CRISPR_CLK_ABS, CRISPR_TIME_SECOND, Crispr_nullRef(Crispr_Errno));
	Crispr_timeConv(&timelock, &timer, CRISPR_CLK_RELA, CRISPR_TIME_SECOND, Crispr_nullRef(Crispr_Errno));
	Crispr_sema_lock(&mutex, false, &timelock, Crispr_nullRef(Crispr_Errno));
	Crispr_sema_unlock(&mutex, Crispr_nullRef(Crispr_Errno));
	Crispr_sema_lock(&mutex, false, Crispr_nullRef(Crispr_Timer), Crispr_nullRef(Crispr_Errno));
	Crispr_Errno err;
	Crispr_sema_destroy(&mutex, &err);
	const char* stat = Crispr_errName(err);
	stat = stat ? stat : "Success.";
	printf("Status: %s\n", stat);
	puts(Crispr_boolStr(Crispr_errIsA(CRISPR_ERRTIMEDOUT, CRISPR_ERRCHNG)));
	puts(Crispr_boolStr(Crispr_errIsA(CRISPR_ERRNOMEM, CRISPR_ERRSYS)));
	puts(Crispr_boolStr(Crispr_errIsA(CRISPR_ERRCHNG, CRISPR_ERRTIMEDOUT)));
	puts(Crispr_boolStr(Crispr_errIsA(CRISPR_ERRTIMEDOUT, CRISPR_ERRSYS)));
	printf("Sema size: %zd\n", sizeof(Crispr_Sema));
	printf("SemSched size: %zd\n", sizeof(Crispr_SemSched));
	printf("Timer size: %zd\n", sizeof(Crispr_Timer));
	printf("Clock size: %zd\n", sizeof(Crispr_Clock));
	exit(0);
}
