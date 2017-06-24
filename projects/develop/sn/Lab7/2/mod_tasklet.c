#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/timex.h>
MODULE_LICENSE("GPL");
cycles_t cycles1, cycles2;
static u32 j1, j2;
char tasklet_data[] = "tasklet_function was called";
/* Bottom Half Function */
void tasklet_function( unsigned long data ) {
	j2 = jiffies; 
	// В ядре Linux время измеряется с помощью глобальной переменной с именем  jiffies, которая определяет количество временных тиков (тактов)
	cycles2 = get_cycles();
	printk( "%010lld [%05d] : %s\n", (long long unsigned)cycles2, j2, (char*)data );
	return;
}

DECLARE_TASKLET( my_tasklet, tasklet_function, (unsigned long)&tasklet_data );

int init_module( void ) {
	j1 = jiffies;
	cycles1 = get_cycles();
	printk( "%010lld [%05d] : tasklet_scheduled\n", (long long unsigned)cycles1, j1 );
	/* Schedule the Bottom Half */
	tasklet_schedule( &my_tasklet );
	return 0;
}
void cleanup_module( void ) {
	/* Stop the tasklet before we exit */
	tasklet_kill( &my_tasklet );
	return;
}
