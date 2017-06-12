#include <sched.h>
#include <stdio.h>

void printPolicy(){
	int policy = sched_getscheduler(0);
	switch (policy) {
	case SCHED_OTHER:
	 	printf ("������� ��������\n");
	 	break;
	case SCHED_RR:
	 	printf ("�������� ��������\n");
	 	break;
	case SCHED_FIFO:
	 	printf("�������� FIFO\n");
	 	break;
	case -1:
	 	printf("sched_getscheduler");
	 	break;
	default:
	 	printf("����������� ��������!\n");
	}
}
int main(){
	printPolicy();
	struct sched_param sp = { .sched_priority = 1 };
	int ret;
	ret = sched_setscheduler(0, SCHED_RR, &sp);
	printPolicy();
	if (sched_yield() == 0)
 		printf("sched_yield\n");
return 0;
}
