#include <CrisprLang/Memory.h>
#include <CrisprLang/Timer.h>
#include <CrisprLang/Sema.h>
#include <CrisprLang/Error.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int main() {
	Crispr_Sema mutex;
	Crispr_sema_init(&mutex, 1, CRISPR_NULL);
	Crispr_sema_lock(&mutex, false, CRISPR_NULL, CRISPR_NULL);
	Crispr_Timer timer;
	Crispr_timeNow(&timer, CRISPR_CLK_RELA, 2, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_Timer timelock;
	Crispr_timeConv(&timelock, &timer, CRISPR_CLK_UTC, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_timeConv(&timer, &timelock, CRISPR_CLK_ABS, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_timeConv(&timelock, &timer, CRISPR_CLK_RELA, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_sema_lock(&mutex, false, &timelock, CRISPR_NULL);
	Crispr_sema_unlock(&mutex, CRISPR_NULL);
	Crispr_sema_lock(&mutex, false, CRISPR_NULL, CRISPR_NULL);
	Crispr_Errno err;
	Crispr_sema_destroy(&mutex, &err);
	const char* stat = Crispr_errName(err);
	stat = stat ? stat : "Success.";
	printf("Status: %s\n", stat);
	printf("is ERRTIMEDOUT a type of ERRINTR: %s\n", Crispr_boolStr(Crispr_errIsA(CRISPR_ERRTIMEDOUT, CRISPR_ERRINTR)));
	printf("is ERRINTR a type of ERRTIMEDOUT: %s\n", Crispr_boolStr(Crispr_errIsA(CRISPR_ERRINTR, CRISPR_ERRTIMEDOUT)));
	printf("is ERRTIMEDOUT a type or ERRSYS: %s\n", Crispr_boolStr(Crispr_errIsA(CRISPR_ERRINTR, CRISPR_ERRSYS)));
	printf("Sema size: %zd\n", sizeof(Crispr_Sema));
	printf("SemSched size: %zd\n", sizeof(Crispr_SemSched));
	printf("Timer size: %zd\n", sizeof(Crispr_Timer));
	printf("Clock size: %zd\n", sizeof(Crispr_Clock));
	exit(0);
}
