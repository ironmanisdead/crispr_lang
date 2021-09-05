#include <CrisprLang/Memory.h>
#include <CrisprLang/Timer.h>
#include <CrisprLang/Sema.h>
#include <CrisprLang/Errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int main() {
	Crispr_t_Sema mutex;
	Crispr_f_sema_init(&mutex, 1, Crispr_M_NULL);
	Crispr_f_sema_lock(&mutex, false, Crispr_M_NULL, Crispr_M_NULL);
	Crispr_t_Timer timer;
	Crispr_f_timeNow(&timer, Crispr_M_CLK_RELA, 2, Crispr_M_TIME_SECOND, Crispr_M_NULL);
	Crispr_t_Timer timelock;
	Crispr_f_timeConv(&timelock, &timer, Crispr_M_CLK_UTC, Crispr_M_TIME_SECOND, Crispr_M_NULL);
	Crispr_f_timeConv(&timer, &timelock, Crispr_M_CLK_ABS, Crispr_M_TIME_SECOND, Crispr_M_NULL);
	Crispr_f_timeConv(&timelock, &timer, Crispr_M_CLK_RELA, Crispr_M_TIME_SECOND, Crispr_M_NULL);
	Crispr_f_sema_lock(&mutex, false, &timelock, Crispr_M_NULL);
	Crispr_f_sema_unlock(&mutex, Crispr_M_NULL);
	Crispr_f_sema_lock(&mutex, false, Crispr_M_NULL, Crispr_M_NULL);
	Crispr_tn_Errno err;
	Crispr_f_sema_destroy(&mutex, &err);
	printf("Status: %s\n", Crispr_f_Errno(err));
	printf("Sema size: %zd\n", sizeof(Crispr_t_Sema));
	printf("SemSched size: %zd\n", sizeof(Crispr_t_SemSched));
	printf("Timer size: %zd\n", sizeof(Crispr_t_Timer));
	printf("Clock size: %zd\n", sizeof(Crispr_t_Clock));
	exit(0);
}
