#include <CrisprLang/Memory.h>
#include <CrisprLang/Timer.h>
#include <CrisprLang/Sema.h>
#include <CrisprLang/Errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int main() {
	Crispr_t_Sema mutex;
	Crispr_f_sema_init(&mutex, 1, CRISPR_NULL);
	Crispr_f_sema_lock(&mutex, false, CRISPR_NULL, CRISPR_NULL);
	Crispr_t_Timer timer;
	Crispr_f_timeNow(&timer, CRISPR_CLK_RELA, 2, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_t_Timer timelock;
	Crispr_f_timeConv(&timelock, &timer, CRISPR_CLK_UTC, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_f_timeConv(&timer, &timelock, CRISPR_CLK_ABS, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_f_timeConv(&timelock, &timer, CRISPR_CLK_RELA, CRISPR_TIME_SECOND, CRISPR_NULL);
	Crispr_f_sema_lock(&mutex, false, &timelock, CRISPR_NULL);
	Crispr_f_sema_unlock(&mutex, CRISPR_NULL);
	Crispr_f_sema_lock(&mutex, false, CRISPR_NULL, CRISPR_NULL);
	Crispr_tn_Errno err;
	Crispr_f_sema_destroy(&mutex, &err);
	printf("Status: %s\n", Crispr_f_Errno(err));
	printf("Sema size: %zd\n", sizeof(Crispr_t_Sema));
	printf("SemSched size: %zd\n", sizeof(Crispr_t_SemSched));
	printf("Timer size: %zd\n", sizeof(Crispr_t_Timer));
	printf("Clock size: %zd\n", sizeof(Crispr_t_Clock));
	exit(0);
}
